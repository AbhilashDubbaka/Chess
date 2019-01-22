#include "ChessBoard.h"
#include "ChessPiece.h"
#include <iostream>

using namespace std;

ChessPiece::ChessPiece(Colour colour, ChessBoard* board)
  : colour(colour), board(board){}

ChessPiece::~ChessPiece(){}

bool ChessPiece::isFileClear(const string source, const string destination){
  if(destination[0] != source[0])
    return false;

  int rank_change = 0;
  if(source[1] < destination[1])
    rank_change = 1;
  else
    rank_change = -1;

  string middle = source;
  middle[1] += rank_change;
  while(middle != destination){
    if(!board->isEmptyPosition(middle))
      return false;
    middle[1] += rank_change;
  }

  return true;
}

bool ChessPiece::isRankClear(const string source, const string destination){
  if(destination[1] != source[1])
    return false;

  int file_change = 0;
  if(source[0] < destination[0])
    file_change = 1;
  else
    file_change = -1;

  string middle = source;
  middle[0] += file_change;
  while(middle != destination){
    if(!board->isEmptyPosition(middle))
      return false;
    middle[0] += file_change;
  }

  return true;
}

bool ChessPiece::isDiagonalClear(const string source, const string destination){
  if(abs(destination[0] - source[0]) != abs(destination[1] - source[1]))
    return false;

  int file_change = 0;
  int rank_change = 0;
  if(source[0] < destination[0])
    file_change = 1;
  else
    file_change = -1;
  if(source[1] < destination[1])
    rank_change = 1;
  else
    rank_change = -1;

  string middle = source;
  middle[0] += file_change;
  middle[1] += rank_change;
  while(middle != destination){
    if(!board->isEmptyPosition(middle))
      return false;
    middle[0] += file_change;
    middle[1] += rank_change;
  }

  return true;
}

Colour ChessPiece::getColour(){
  return colour;
}

Type ChessPiece::getType(){
  return type;
}

string ChessPiece::getSymbol(){
  return symbol;
}

string ChessPiece::printType(){
  switch(type){
    case Type::King:
      return "King";
    case Type::Queen:
      return "Queen";
    case Type::Bishop:
      return "Bishop";
    case Type::Knight:
      return "Knight";
    case Type::Rook:
      return "Rook";
    case Type::Pawn:
      return "Pawn";
    default:
      return "NULL";
  }
}

string ChessPiece::printColour(){
  switch(colour){
    case Colour::White:
      return "White";
    case Colour::Black:
      return "Black";
    default:
      return "NULL";
  }
}
