#ifndef PAWN_H
#define PAWN_H

#include "Errors.h"
#include "Helper.h"
#include "ChessPiece.h"
#include <string>

/* Forward declaration of the chessboard class */
class ChessBoard;

/* Class for the Pawn piece */
class Pawn : public ChessPiece{

 private:
  /* Data member that is true if the pawn hasn't made its first move yet */
  bool is_first_move;

  /* Data member that is true if the pawn has just made two steps forward so
     is open for an attack from the enemy via En Passant */
  bool is_en_passant_available;

  /* Data member that is true if the pawn is making attacking an enemy 
     pawn using the En Passant move */
  bool is_en_passant_attack;

 public:
  /* Constructor that initialises the colour and type of piece to Pawn. It
     sets is_first_move to true, is_en_passant_available to false, 
     is_en_passant_attack to false and gets the correct symbol of the piece, 
     used for printing the chessboard */
  Pawn(Colour colour, ChessBoard* board);

  /* Default destructor */
  ~Pawn();
  
  /* Returns true if the move is valid using string inputs for source and 
     destination positions, and false otherwise. For the Pawn, it is a 
     valid move if it:
     - moves one square forward and there is no piece at that destination; 
     - moves two squares forward on its first move only and no pieces are 
     either in square in front or at the destination square;  
     - if it moves diagonally one square forward to capture an enemy either
     via a normal move so it captures the enemy piece, which is at the 
     destination square or via En Passant, which is explained further below
     in the isEnPassant function 
     
     Also, the destination must either be an empty square or 
     contains an enemy piece and it will not put the king in check after 
     the move. Note that this does not check if the move puts the king in 
     check, which is done in a later function. Also, it does not check if 
     the input is valid or if the destination square has a friendly piece 
     or that the source and destination inputs are different to each other, 
     which are also done in another function */
  bool isValidMove(const std::string source, const std::string destination) override;

  /* Using a string input for source and destination, it returns true if the 
     En Passant move is valid. This is a special pawn capture move that can 
     only occur immediately after a pawn moves two squares forward on its 
     first turn, an enemy pawn can go diagonally forward one step to where 
     the pawn would have been if it only took one step forward and capture 
     the pawn that moved two squares. This attack can only take place by a 
     White pawn that is on rank 5 or a Black pawn that is on rank 4. If the 
     en passant capture is not made on the very next turn after the pawn took 
     two steps forwards then the right to do the En Passant is lost and it 
     cannot be done again on that pawn that took two steps forward */
  bool isEnPassant(const std::string source, const std::string destination);

  /* "Setter" function to set is_first_move to false once it has made its 
     first move */
  void setFirstMoveToFalse();

  /* "Setter" function to set is_en_passant_available to true or false depending 
     on the bool value that it takes in. Used to keep account of if a 
     En Passant is available for an enemy pawn to do */
  void setEnPassantAvailable(bool value);

  /* "Setter" function to set is_en_passant_attack to true or false depending 
     on the bool value that it takes in. Used to keep account of if the pawn 
     is using an En Passant move */
  void setEnPassantAttack(bool value);

  /* "Getter" function to return is_first_move data member */
  bool getIsFirstMove();
  
  /* "Getter" function to return is_en_passant_available data member */
  bool getIsEnPassantAvailable();

  /* "Getter" function to return is_en_passant_attack data member */
  bool getIsEnPassantAttack();

};

#endif
