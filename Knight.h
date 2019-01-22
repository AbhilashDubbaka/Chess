#ifndef KNIGHT_H
#define KNIGHT_H

#include "Errors.h"
#include "Helper.h"
#include "ChessPiece.h"
#include <string>

/* Forward declaration of the chessboard class */
class ChessBoard;

/* Class for the Knight piece */
class Knight : public ChessPiece{

 private:

 public:
  /* Constructor that initialises the colour and type of piece to Knight. It 
     gets the correct symbol of the piece, used for printing the chessboard */
  Knight(Colour colour, ChessBoard* board);

  /* Default destructor */
  ~Knight();

  /* Returns true if the move is valid using string inputs for source and 
     destination positions, and false otherwise. For the Knight, it is a valid
     move if it moves two squares on the rank and one square on the file or 
     if it moves two squares on the file and one square on the rank. It can 
     "jump over" other pieces. Also, the destination must either be an empty 
     square or contains an enemy piece and it will not put the king in check 
     after the move. Note that this does not check if the move puts the king 
     in check, which is done in a later function. Also, it does not check if 
     the input is valid or if the destination square has a friendly piece or 
     that the source and destination inputs are different to each other, which 
     are also done in another function */
  bool isValidMove(const std::string source, const std::string destination) override;

};

#endif
