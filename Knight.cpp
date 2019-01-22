#include "Knight.h"
#include <iostream>

using namespace std;

Knight::Knight(Colour colour, ChessBoard* board)
  : ChessPiece(colour, board){

  type = Type::Knight;
  if(colour == Colour::White)
    symbol = "♘";
  else
    symbol = "♞";
}

Knight::~Knight(){}

bool Knight::isValidMove(const string source, const string destination){
  /* Moves two squares horizontal and one square vertical */
  if((abs(destination[0] - source[0]) == 2) && (abs(destination[1] - source[1]) == 1))
    return true;

  /* Moves one square horizontal and two squares vertical */
  if((abs(destination[0] - source[0]) == 1) && (abs(destination[1] - source[1]) == 2))
    return true;

  return false;

}
