#include "Queen.h"
#include <iostream>

using namespace std;

Queen::Queen(Colour colour, ChessBoard* board)
  : ChessPiece(colour, board){

  type = Type::Queen;
  if(colour == Colour::White)
    symbol = "♕";
  else
    symbol = "♛";
}

Queen::~Queen(){}

bool Queen::isValidMove(const string source, const string destination){
  /* Move along the rank */
  if(isRankClear(source, destination))
    return true;

  /* Move along the file */
  if(isFileClear(source, destination))
    return true;

  /* Move along the diagonal */
  if(isDiagonalClear(source, destination))
    return true;

  return false;
}
