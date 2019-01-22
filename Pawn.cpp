#include "Pawn.h"
#include <iostream>

using namespace std;

Pawn::Pawn(Colour colour, ChessBoard* board)
  : ChessPiece(colour, board){

  type = Type::Pawn;
  is_first_move = true;
  is_en_passant_available = false;
  is_en_passant_attack = false;
  if(colour == Colour::White)
    symbol = "♙";
  else
    symbol = "♟";
}

Pawn::~Pawn(){}

bool Pawn::isValidMove(const string source, const string destination){
  int rank_change = 0;
  if(colour == Colour::White)
    rank_change += 1;
  else
    rank_change -= 1;

  /* If it is the first move for this pawn, then it can move two steps forward */
  if(is_first_move && (destination[0] == source[0]) &&
     (destination[1] == source[1] + rank_change * 2)){
    string middle = source;
    middle[1] += rank_change;
    if(board->isEmptyPosition(middle) && board->isEmptyPosition(destination))
      return true;
  }

  /* Moving the pawn one step forward */
  if((destination[0] == source[0]) && (destination[1] == source[1] + rank_change)){
    if(board->isEmptyPosition(destination))
      return true;
  }

  /* Moving diagonally and capturing an enemy piece */
  if((abs(destination[0] - source[0]) == 1) && (destination[1] == source[1] + rank_change)){
    if(board->isEnemyPiece(destination, colour))
      return true;
  }

  /* En Passant Special Move */
  if(isEnPassant(source, destination))
    return true;

  return false;
}

bool Pawn::isEnPassant(const string source, const string destination){
  int rank_change = 0;
  if(colour == Colour::White)
    rank_change += 1;
  else
    rank_change -= 1;

  /* Checks that if the Pawn is white, then it is on rank 5 or if its black,
     its on rank 4 */
  if((colour == Colour::White && source[1] == EN_PASSANT_WHITE_ATTACK_RANK) ||
     (colour == Colour::Black && source[1] == EN_PASSANT_BLACK_ATTACK_RANK)){

    /* Checks if the pawn is moving diagonally and its moving to an empty
       destination square */
    if((abs(destination[0] - source[0]) == 1) &&
       (destination[1] == source[1] + rank_change) &&
       board->isEmptyPosition(destination)){

      string enemy_pawn_source = destination;
      enemy_pawn_source[1] = source[1];
      
      /* Check that the there is an enemy pawn where it is supposed to be
	       for an En Passant to take place and that it can still be captured
	       via an En Passant move */
      if((!board->isEmptyPosition(enemy_pawn_source)) &&
         (board->isEnPassantValid(enemy_pawn_source, colour))){
        return true;
      }
    }
  }

  return false;
}


bool Pawn::getIsEnPassantAvailable(){
  return is_en_passant_available;
}

bool Pawn::getIsEnPassantAttack(){
  return is_en_passant_attack;
}

void Pawn::setFirstMoveToFalse(){
  is_first_move = false;
}

void Pawn::setEnPassantAvailable(bool value){
  is_en_passant_available = value;
}

void Pawn::setEnPassantAttack(bool value){
  is_en_passant_attack = value;
}

bool Pawn::getIsFirstMove(){
  return is_first_move;
}
