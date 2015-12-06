
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

const int MAX_ARRAY_SIZE = 24;

bool InitializeBoard(int  CaptureMeBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE],  int numRowsInBoard );
bool PlacePiece(int CaptureMeBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard,  int player, int squarePlayed, int whichPiece, int& numberCaptured);
bool DisplayBoard(int CaptureMeBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE],  int numRowsInBoard);
bool CheckLine( int CaptureMeBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int squarePlayed, int xDirection, int yDirection, int& captured );
bool IsMove(int CaptureMeBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player);

struct player
{
	int id;
	char displayName;
	string textName;
	bool canMove;
};

int main()
{
	int myCaptureMeBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE];
	int numRowsInBoard = 0;
	//int player = 1; //1 = white     2 = black
	const int BOARD_SIZE_MAX_RETRY = 3;
	const int MAX_BOARD_SIZE = 24;
	const int MIN_BOARD_SIZE = 8;
	bool gameOver = false;
	int squarePlace = 0;
	int numCaptured = 0;
	int whichPiece = 1;
	int totalTurns = 0;
	int maxTurns = 0;
	int i = 0;
	char endGameKey = '\0';
	char YesNoAns = '\0';
	struct player white;
	white.displayName = 'W';
	white.id = 1;
	white.textName = "White";
	white.canMove = true;
	struct player black;
	black.displayName = 'B';
	black.id = 2;
	black.textName = "Black";
	black.canMove = true;
	struct player currentPlayer = white; //start game off with white player

	for (int i = 0; i < BOARD_SIZE_MAX_RETRY+1; i++)
	{
		cout << "Enter the number of squares along each edge of the board\n";
		if(!(cin >> numRowsInBoard))
		{
			cerr << "ERROR:  Board size is not an integer" << endl;
			cin.clear();
			cin.sync();
		}
		else
		{
			if(numRowsInBoard > MAX_BOARD_SIZE)
			{
				cerr << "ERROR:  Board size too large" << endl;
			}
			else if(numRowsInBoard < MIN_BOARD_SIZE)
			{
				cerr << "ERROR:  Board size too small" << endl;
			}
			else if(numRowsInBoard % 2 != 0)
			{
				cerr << "ERROR:  Board size odd" << endl;
			}
			else{break;}
		}
		cout << "8 <= number of squares <= 24" << endl;
		if(i > BOARD_SIZE_MAX_RETRY-1)
		{
			cerr << "ERROR:  Too many errors entering the size of the board" << endl;
			return 0;
		}
		
	}

	maxTurns = numRowsInBoard*numRowsInBoard;

	InitializeBoard(myCaptureMeBoard,numRowsInBoard);
	DisplayBoard(myCaptureMeBoard,numRowsInBoard);
	cout<<"\n\n";

	//begin gameplay-----------------------------------------------------------------------------------------
	while(gameOver==false)
	{

		if(IsMove(myCaptureMeBoard, numRowsInBoard, currentPlayer.id))
		{
			cout<<currentPlayer.textName<<" takes a turn\n";
			whichPiece = 1;

			i=0;
			while (i < 4) //allow user to attempt a move
			{
				if(i==3)
				{
					cout<<currentPlayer.textName<<" has forfeited a turn\n";
					DisplayBoard(myCaptureMeBoard,numRowsInBoard);
					cout<<"\n\n";
					break;
				}

				cout<<"Enter square number where you want to put your";
				if(whichPiece==1)
				{
					cout<<" first piece\n";
				}
				else if(whichPiece==2)
				{
					cout << " second piece\n";
				}
				else
				{
					cerr<<"ERROR:Invalid number of moves for one player\n";
				}

				if(!(cin>>squarePlace))
				{
					cin.clear();
					cin.sync();
					cerr<<"ERROR:  that square is not on the board.\nTry again\n";
					i++;
					continue;
				}

				if(squarePlace>maxTurns-1 || squarePlace<0) //potition is outside of board
				{
					cerr<<"ERROR:  that square is not on the board. \nTry again\n";
					i++;
					continue;
				}
				if(myCaptureMeBoard[squarePlace/numRowsInBoard][squarePlace%numRowsInBoard]!=0) //place is already placed at selected position
				{
					cerr<<"ERROR:  that square is already occupied\nTry again\n";
					i++;
					continue;
				}

				if(PlacePiece(myCaptureMeBoard, numRowsInBoard, currentPlayer.id, squarePlace, whichPiece, numCaptured)) //attempt to place piece on the board
				{
					if(!DisplayBoard(myCaptureMeBoard,numRowsInBoard))
					{
						cerr<<"ERROR:board did not display correctly\n";
					}
					cout<<"\n\n";
				}
				else
				{
					//illigal attempt to place piece
					cerr<<"ERROR:Could not place piece\n";
					i++;
					continue;
				}
				
				totalTurns++;

				if(numCaptured >= 2 && whichPiece < 3 && IsMove(myCaptureMeBoard, numRowsInBoard, currentPlayer.id))
				{
					cout<<"Do you wish to play a second piece?  Y/N ";
					if(!(cin>>YesNoAns))
					{
						cerr<<"ERROR: input not read properly\n";
					}
					if(YesNoAns=='Y' || YesNoAns=='y')
					{
						i = 0; //reset counter and start from biggining
						whichPiece=2;
						continue;
					}
				}
				else {break;} //turn completed, got to next player 

				i++;
			}
			currentPlayer.canMove = true;
		}
		else
		{
			cout<<currentPlayer.textName<<" is unable to move\n";
			currentPlayer.canMove = false;
		}


		//switching between the two players every turn
		if(currentPlayer.id == white.id)
		{
			white.canMove = currentPlayer.canMove;
			currentPlayer = black;
		}
		else if(currentPlayer.id == black.id)
		{
			black.canMove = currentPlayer.canMove;
			currentPlayer = white;
		}

		//determin condition for game termination
		if(totalTurns >= maxTurns || (!white.canMove && !black.canMove))
		{
			cout<<"GAME OVER\n";
			cout<<"Please enter any character  to close the game window ";
			cin >>endGameKey;
			break;
		}
	}


	return 0;
}



bool InitializeBoard(int  CaptureMeBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE],  int numRowsInBoard )
{
	int i=0,j=0,k=0;//index

	if(numRowsInBoard>24||numRowsInBoard<8||numRowsInBoard%2!=0)
	{
		return false;
	}

	for(i=0;i<numRowsInBoard;i++)
	{
		for(k=0;k<numRowsInBoard;k++)
		{
			CaptureMeBoard[i][k]=0;
		}
		
	}
	CaptureMeBoard[numRowsInBoard/2][numRowsInBoard/2]=1;
	CaptureMeBoard[numRowsInBoard/2-1][numRowsInBoard/2-1]=1;
	CaptureMeBoard[numRowsInBoard/2-1][numRowsInBoard/2]=2;
	CaptureMeBoard[numRowsInBoard/2][numRowsInBoard/2-1]=2;

	return true;
}

bool PlacePiece(int CaptureMeBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard,  int player, int squarePlayed, int whichPiece, int& numberCaptured)
{
	int boardX = squarePlayed%numRowsInBoard;
	int boardY = squarePlayed/numRowsInBoard;
	int numberCapturedPerLine = 0;
	bool didCapture = false;
	const int ORTHOGONAL_DIRECTIONS[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
	const int DIAGNOL_DIRECTIONS[4][2] = {{1,1},{-1,-1},{1,-1},{-1,1}};
	numberCaptured = 0;



	if(whichPiece == 1)
	{
		for (int i = 0; i < 4; i++)
		{
			if(CheckLine(CaptureMeBoard, numRowsInBoard, player, squarePlayed, ORTHOGONAL_DIRECTIONS[i][0], ORTHOGONAL_DIRECTIONS[i][1], numberCapturedPerLine)){
				didCapture = true;
				numberCaptured += numberCapturedPerLine;
			}
		}
	}
	else if(whichPiece == 2)
	{
		for (int i = 0; i < 4; i++)
		{
			if(CheckLine(CaptureMeBoard, numRowsInBoard, player, squarePlayed, DIAGNOL_DIRECTIONS[i][0], DIAGNOL_DIRECTIONS[i][1], numberCapturedPerLine)){
				didCapture = true;
			}
		}
	}
	else
	{
		cerr << "ERROR:move number is off\n";
	}
		
	if(didCapture)
	{
		CaptureMeBoard[boardY][boardX] = player;
		if(player == 1)
		{
			cout << "White has placed the first piece" << endl;
		}
		else if(player == 2)
		{
			cout << "Black has placed the first piece" << endl;
		}
		else
		{
			cerr << "ERROR:player does not exist" << endl;
		}
		return true;
	}
	else
	{
		cout << "Illegal move" << endl;
		return false;
	}
}
	
	
bool DisplayBoard( int CaptureMeBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE],  int numRowsInBoard)
{
	int i=0,j=0,k=0;//index
	cout << fixed;
	for(i=0;i<numRowsInBoard;i++)
	{
		for(k=0;k<numRowsInBoard;k++)
		{
			if(CaptureMeBoard[i][k]==0)
			{
				cout << setw(4) << j;
			}
			if(CaptureMeBoard[i][k]==1)
			{
				cout << setw(4) << "W";
			}
			if(CaptureMeBoard[i][k]==2)
			{
				cout << setw(4) << "B";
			}
			j++;
		}
		cout << endl;
	}
	return true;
}
	
	

bool CheckLine( int CaptureMeBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, 
			   int player, int squarePlayed, 
			   int xDirection, int yDirection, int& captured )
{    
	// This function takes the CaptureMeBoard and calculates
	// how the board changes when player player
	// (player=1 for white, player=2 for black)
	// places a piece in square, squarePlayed of the board
	// This function checks along a row in one direction for
	// al legal move. If no legal move is found this function
	// returns false, if a legal move is found this function
	// returns true.
	// Author Janice Regan; November 2, 2007
	int otherPlayer;
	char piece = 2;           // to compare for otherPlayers
	int xIndex = 0;
	int yIndex = 0;
	int xCoordinate = 0;
	int yCoordinate = 0;
	bool edge = true;

	otherPlayer = 1;
	if( player == 1 ) 
	{ 
		otherPlayer = 2; 
	}

	// find row and column were new piece may be placed
	xCoordinate = squarePlayed % numRowsInBoard;
	yCoordinate = squarePlayed / numRowsInBoard;

	// find the square next to the location the piece may
	// be played. Next in the direction (xDirection,yDirection)
	xIndex = xCoordinate+xDirection;
	yIndex = yCoordinate+yDirection;

	// while the square to be checked is on the board 
	// continue looking for a legal move
	// If the square is not on the board then return false
	// there is no legal move in the indicated direction
	while( ( xIndex < numRowsInBoard && 
		yIndex < numRowsInBoard ) &&
		( xIndex >= 0 && yIndex >=0 ) )
	{
		//if the location where the piece is to be placed
		//is next to an empty square or to another of the
		//player's pieces, then the there is no legal
		// move in the direction indicated 
		// return false
		if(CaptureMeBoard[yIndex][xIndex] == 0 ||
			CaptureMeBoard[yIndex][xIndex] == player)
		{
			return false;
		}
		else
		{
			// The next piece in the direction indicated is a
			// opponent's piece. The move may be legal, proceed
			// in the indicated direction and try to find a players
			// piece.  If one is found before the edge of the board
			// is reached the move is legal
			// If a legal move is found, make the move
			edge = true;
			while(yIndex < numRowsInBoard &&
				xIndex < numRowsInBoard &&
				yIndex >= 0 && xIndex >= 0)
			{
				edge = false;
				if( CaptureMeBoard[yIndex][xIndex] == 0)
				{
					//next square along the indicated direction is empty
					//move is illegal
					return false;
				}
				if( CaptureMeBoard[yIndex][xIndex] == player)
				{
					//next square along the direction indicated hold
					//one of the players pieces.  The move is legal
					break;
				}
				else
				{				
					//next square along the direction indicated holds
					//an opponents piece, continue along the same 
					//direction and try the next square 
					xIndex += xDirection;
					yIndex += yDirection;
					edge = true;
				}
			}


			if(edge == true)
			{
				//The edge of the board has been reached without 
				//finding a players piece, move is not legal
				return false;
			}


			//The move is legal, 
			//Place your new game piece
			CaptureMeBoard[yCoordinate][xCoordinate] = player;
			xIndex = xCoordinate+xDirection;
			yIndex = yCoordinate+yDirection;

			// Change the colour of the opponents trapped
			// pieces.  Then return succesfully to calling program
			while(CaptureMeBoard[yIndex][xIndex] == otherPlayer)
			{
				CaptureMeBoard[yIndex][xIndex] = player;
				xIndex += xDirection;
				yIndex += yDirection;
				captured++;
			}
			return true;
		}
	}
	//The edge of the board has been reached,
	//no legal move.  return false
	return false;
}
	
	
bool IsMove( int CaptureMeBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player)
{
	int i=0,j=0,k=0;
	//checking for whites turn
	if(player==1)
	{
		for(i=0;i<numRowsInBoard;i++)
		{
			for(k=0;k<numRowsInBoard;k++)
			{
				if(CaptureMeBoard[i][k]!=2)
				{
					continue;
				}
				else
				{
					if(CaptureMeBoard[i+1][k]== 1 && CaptureMeBoard[i-1][k]== 0)
					{
						return true;
					}
					if(CaptureMeBoard[i-1][k]== 1 && CaptureMeBoard[i+1][k]== 0)
					{
						return true;
					}
					if(CaptureMeBoard[i][k+1]== 1 && CaptureMeBoard[i][k-1]== 0)
					{
						return true;
					}
					if(CaptureMeBoard[i][k-1]== 1 && CaptureMeBoard[i][k+1]== 0)
					{
						return true;
					}
					if(CaptureMeBoard[i-1][k-1]== 1 && CaptureMeBoard[i+1][k+1]== 0)
					{
						return true;
					}
					if(CaptureMeBoard[i+1][k+1]== 1 && CaptureMeBoard[i-1][k-1]== 0)
					{
						return true;
					}
					if(CaptureMeBoard[i-1][k+1]== 1 && CaptureMeBoard[i+1][k-1]== 0)
					{
						return true;
					}
					if(CaptureMeBoard[i+1][k-1]== 1 && CaptureMeBoard[i-1][k+1]== 0)
					{
						return true;
					}
				}
			}
		}
		cout << "White is unable to move\n";
		return false;
	}
	//checking for blacks turn
	else if(player==2)
	{
		for(i=0;i<numRowsInBoard;i++)
		{
			for(k=0;k<numRowsInBoard;k++)
			{
				if(CaptureMeBoard[i][k]!=1)
				{
					continue;
				}
				else
				{
					if(CaptureMeBoard[i+1][k]== 2 && CaptureMeBoard[i-1][k]== 0)
					{
						return true;
					}
					if(CaptureMeBoard[i-1][k]== 2 && CaptureMeBoard[i+1][k]== 0)
					{
						return true;
					}
					if(CaptureMeBoard[i][k+1]== 2 && CaptureMeBoard[i][k-1]== 0)
					{
						return true;
					}
					if(CaptureMeBoard[i][k-1]== 2 && CaptureMeBoard[i][k+1]== 0)
					{
						return true;
					}
					if(CaptureMeBoard[i-1][k-1]== 2 && CaptureMeBoard[i+1][k+1]== 0)
					{
						return true;
					}
					if(CaptureMeBoard[i+1][k+1]== 2 && CaptureMeBoard[i-1][k-1]== 0)
					{
						return true;
					}
					if(CaptureMeBoard[i-1][k+1]== 2 && CaptureMeBoard[i+1][k-1]== 0)
					{
						return true;
					}
					if(CaptureMeBoard[i+1][k-1]== 2 && CaptureMeBoard[i-1][k+1]== 0)
					{
						return true;
					}
				}
			}
		}
		cout << "Black is unable to move\n";
		return false;
	}
	else
	{
		cerr<<"ERROR:player does not exist\n";
		return false;
	}
}