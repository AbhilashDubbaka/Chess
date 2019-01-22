#include "Bishop.h"
#include <iostream>

using namespace std;

Bishop::Bishop(Colour colour, ChessBoard* board)
  : ChessPiece(colour, board){

  type = Type::Bishop;
  if(colour == Colour::White)
    symbol = "♗";
  else
    symbol = "♝";
}

Bishop::~Bishop(){}

bool Bishop::isValidMove(const string source, const string destination){
  /* Move along the diagonal */
  if(isDiagonalClear(source, destination))
    return true;
  return false;
}
