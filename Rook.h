#ifndef ROOK_H
#define ROOK_H

#include "Errors.h"
#include "Helper.h"
#include "ChessPiece.h"
#include <string>

/* Forward declaration of the chessboard class */
class ChessBoard;

/* Class for the Rook piece */
class Rook : public ChessPiece{

 private:
  /* Data member that is true if the rook hasn't made its first move yet */
  bool is_first_move;

 public:
  /* Constructor that initialises the colour and type of piece to Rook. It
     sets is_first_move to true and gets the correct symbol of the piece, 
     used for printing the chessboard */
  Rook(Colour colour, ChessBoard* board);

  /* Default destructor */
  ~Rook();
  
  /* Returns true if the move is valid using string inputs for source and 
     destination positions, and false otherwise. For the Rook, it is a 
     valid move if it moves horizontally or vertically across the board i.e. 
     the source is on the same file or rank as the destination square, and 
     there are no other pieces on this path so it cannot "jump over" other 
     pieces. Also, the destination must either be an empty square or 
     contains an enemy piece and it will not put the king in check after 
     the move. Note that this does not check if the move puts the king in 
     check, which is done in a later function. Also, it does not check if 
     the input is valid or if the destination square has a friendly piece 
     or that the source and destination inputs are different to each other, 
     which are also done in another function */
  bool isValidMove(const std::string source, const std::string destination) override;

  /* "Setter" function to set is_first_move to false once it has made its 
     first move */
  void setFirstMoveToFalse();

  /* "Getter" function to return is_first_move data member */
  bool getIsFirstMove();

};

#endif
