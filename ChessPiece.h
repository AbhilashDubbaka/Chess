#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include "Errors.h"
#include "Helper.h"
#include "ChessBoard.h"
#include <string>

/* Forward declaration of the chessboard class */
class ChessBoard;

/* An abstract base class for all the chess pieces */
class ChessPiece{
 protected:
  /* Used to identify the colour of the piece, either White or Black */
  Colour colour;

  /* Used to point to the board, which contains all the other pieces */
  ChessBoard* board;

  /* Used to specify the type of piece, e.g. King, Queen etc. */
  Type type;

  /* Used to get the Unicode chess piece symbol for printing the board */
  std::string symbol;

  /* Takes in strings for the source and destination positions and 
     returns true if there is no piece in between the source and 
     destination positions in the same rank */
  bool isRankClear(const std::string source, const std::string destination);

  /* Takes in strings for the source and destination positions and 
     returns true if there is no piece in between the source and 
     destination positions in the same file */
  bool isFileClear(const std::string source, const std::string destination);

  /* Takes in strings for the source and destination positions and 
     returns true if there is no piece in between the source and 
     destination positions in a diagonal path */
  bool isDiagonalClear(const std::string source, const std::string destination);

 public:
  /* Constructor for a chess piece requires a colour of the piece and 
     a pointer pointing to the chessboard object */
  ChessPiece(Colour colour, ChessBoard* board);

  /* Default destructor */
  virtual ~ChessPiece();

  /* Pure virtual member function that checks if a move is valid. This 
     is different for each piece, hence it is virtual */ 
  virtual bool isValidMove(const std::string source, const std::string destination) = 0;

  /* "Getter" functions to return data members */
  Colour getColour();
  Type getType();
  std::string getSymbol();

  /* "Printer" functions to print strings for data members */
  std::string printType();
  std::string printColour();
};

#endif
