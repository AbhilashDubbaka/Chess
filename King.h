#ifndef KING_H
#define KING_H

#include "Errors.h"
#include "Helper.h"
#include "ChessPiece.h"
#include <string>

/* Forward declaration of the chessboard class */
class ChessBoard;

/* Class for the King piece */
class King : public ChessPiece{

 private:
  /* Data member that is true if the king hasn't made its first move yet */
  bool is_first_move;

  /* Data member that is true if the king is making a castling move */
  bool is_castling_move;

 public:
   /* Constructor that initialises the colour and type of piece to King. It
      sets is_first_move to true and is_castling_move to false and gets the
      correct symbol of the piece, used for printing the chessboard */
  King(Colour colour, ChessBoard* board);

  /* Default destructor */
  ~King();

  /* Returns true if the move is valid using string inputs for source and 
     destination positions, and false otherwise. For the King, it is a valid
     move if it moves one square in any direction, provided that it is an 
     empty square or contains an enemy piece and it will not be in check at
     the destination position. It can also perform the castling move under
     specific circumstances, explained in the isCastling function below.
     Note that this does not check if the move puts the king in check, which
     is done in a later function. Also, it does not check if the input is
     valid or if the destination square has a friendly piece or that the 
     source and destination inputs are different to each other, which are 
     also done in another function */
  bool isValidMove(const std::string source, const std::string destination) override;

  /* A castling move is when the king takes two spaces towards the rook it 
     is castling with and the rook then "jumps over" the king and takes 
     position on the other side of the king, all on the same file. It is a
     valid move if the king and the chosen rook have not been previously 
     moved, there are no pieces between the king and rook, the king is not
     currently in check, does not pass through a square that is attacked by 
     an enemy piece and does not end up in check. This function checks for
     all those conditions and returns true if all the conditions are met */
  bool isCastling(const std::string source, const std::string destination);

  /* "Setter" function to set is_first_move to false once it has made its 
     first move */
  void setFirstMoveToFalse();

  /* "Setter" function to set is_castling_move to true or false depending 
     on the bool value that it takes in. Used to keep account of if a 
     castling move is taking place */
  void setIsCastlingMove(bool value);

  /* "Getter" function to return is_first_move data member */
  bool getIsFirstMove();

  /* "Getter" function to return is_castling_move data member */
  bool getIsCastlingMove();

};

#endif
