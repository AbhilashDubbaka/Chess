#include "King.h"
#include <iostream>

using namespace std;

King::King(Colour colour, ChessBoard* board)
  : ChessPiece(colour, board){

  type = Type::King;
  is_first_move = true;
  is_castling_move = false;
  if(colour == Colour::White)
    symbol = "♔";
  else
    symbol = "♚";
}

King::~King(){}

bool King::isValidMove(const string source, const string destination){
  /* Can move one square in any direction */
  if(((abs(destination[0] - source[0]) == 1) && (abs(destination[1] - source[1]) <= 1)) ||
     ((abs(destination[0] - source[0]) == 0) && (abs(destination[1] - source[1]) == 1))){
    return true;
  }

  /* Castling with Rook */
  if(isCastling(source, destination))
    return true;

  return false;
}

bool King::isCastling(const string source, const string destination){
  /* Checks the king is moving two squares on the file and it is the king's
     first move and destination square is empty */
  if((abs(destination[0] - source[0]) == 2) && (abs(destination[1] - source[1]) == 0) &&
     (is_first_move) && (board->isEmptyPosition(destination))){

    string rook_source = destination;
    string rook_destination = destination;
    if(source[0] < destination[0]){
      rook_source[0] = FILE_MAX;
      rook_destination[0] -= 1;
    }
    else{
      rook_source[0] = FILE_MIN;
      rook_destination[0] += 1;
    }

    string king_source = source;
    string king_destination = destination;

    /* Checks the rank is clear between the king and the rook and if it
       satisfies the other Castling conditions */
    if((isRankClear(king_source, rook_source)) &&
       (board->isCastlingValid(king_source, king_destination, rook_source, rook_destination))){
      return true;
    }

  }

  return false;
}

void King::setFirstMoveToFalse(){
  is_first_move = false;
}

void King::setIsCastlingMove(bool value){
  is_castling_move = value;
}

bool King::getIsFirstMove(){
  return is_first_move;
}

bool King::getIsCastlingMove(){
  return is_castling_move;
}
