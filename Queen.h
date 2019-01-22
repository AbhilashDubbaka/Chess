#ifndef QUEEN_H
#define QUEEN_H

#include "Errors.h"
#include "Helper.h"
#include "ChessPiece.h"
#include <string>

/* Forward declaration of the chessboard class */
class ChessBoard;

/* Class for the Queen piece */
class Queen : public ChessPiece{

 private:

 public:
  /* Constructor that initialises the colour and type of piece to Queen. It 
     gets the correct symbol of the piece, used for printing the chessboard */
  Queen(Colour colour, ChessBoard* board);

  /* Default destructor */
  ~Queen();
  
  /* Returns true if the move is valid using string inputs for source and 
     destination positions, and false otherwise. For the Queen, it is a 
     valid move if it moves horizontally or vertically or diagonally across 
     the board i.e. the source is on the same file or rank or diagonal path 
     as the destination square, and there are no other pieces on this path 
     so it cannot "jump over" other pieces. Also, the destination must 
     either be an empty square or contains an enemy piece and it will not 
     put the king in check after the move. Note that this does not check if 
     the move puts the king in check, which is done in a later function. Also, 
     it does not check if the input is valid or if the destination square has 
     a friendly piece or that the source and destination inputs are different 
     to each other, which are also done in another function */
  bool isValidMove(const std::string source, const std::string destination) override;

};

#endif
