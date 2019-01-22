#include "Rook.h"
#include <iostream>

using namespace std;

Rook::Rook(Colour colour, ChessBoard* board)
  : ChessPiece(colour, board){

  type = Type::Rook;
  is_first_move = true;
  if(colour == Colour::White)
    symbol = "♖";
  else
    symbol = "♜";
}

Rook::~Rook(){}

bool Rook::isValidMove(const string source, const string destination){
  /* Move along the rank */
  if(isRankClear(source, destination))
    return true;

  /* Move along the file */
  if(isFileClear(source, destination))
    return true;

  return false;
}

void Rook::setFirstMoveToFalse(){
  is_first_move = false;
}

bool Rook::getIsFirstMove(){
  return is_first_move;
}
