#include "King.h"
#include "Queen.h"
#include "Bishop.h"
#include "Knight.h"
#include "Rook.h"
#include "Pawn.h"
#include "ChessBoard.h"

#include <iostream>

using namespace std;

ChessBoard::ChessBoard(){
  initialiseBoard();
}

void ChessBoard::initialiseBoard(){
  is_game_running = true;
  player_turn = Colour::White;
  white_king_position = "E1";
  black_king_position = "E8";
  captured_piece = nullptr;

  string pawn_position;

  position["A1"] = new Rook(Colour::White, this);
  position["B1"] = new Knight(Colour::White, this);
  position["C1"] = new Bishop(Colour::White, this);
  position["D1"] = new Queen(Colour::White, this);
  position["E1"] = new King(Colour::White, this);
  position["F1"] = new Bishop(Colour::White, this);
  position["G1"] = new Knight(Colour::White, this);
  position["H1"] = new Rook(Colour::White, this);
  for(pawn_position = "A2"; pawn_position[0] <= FILE_MAX; pawn_position[0]++)
    position[pawn_position] = new Pawn(Colour::White, this);

  position["A8"] = new Rook(Colour::Black, this);
  position["B8"] = new Knight(Colour::Black, this);
  position["C8"] = new Bishop(Colour::Black, this);
  position["D8"] = new Queen(Colour::Black, this);
  position["E8"] = new King(Colour::Black, this);
  position["F8"] = new Bishop(Colour::Black, this);
  position["G8"] = new Knight(Colour::Black, this);
  position["H8"] = new Rook(Colour::Black, this);
  for(pawn_position = "A7"; pawn_position[0] <= FILE_MAX; pawn_position[0]++)
    position[pawn_position] = new Pawn(Colour::Black, this);

  for(char rank = '3'; rank <= '6'; rank++){
    for(char file = FILE_MIN; file <= FILE_MAX; file++)
      position[string(1, file) + string(1, rank)] = nullptr;
  }

  cout << "A new chess game is started!" << endl;
  //printBoard();
  error_code = SUCCESS;
}

void ChessBoard::submitMove(const string source, const string destination){
  error_code = SUCCESS;

  if(is_game_running == false){
    error_code = ERROR_GAME_NOT_RUNNING;
    printError(source, destination);
    return;
  }

  if(!isValidInput(source, destination)){
    printError(source, destination);
    return;
  }

  if(!position[source]->isValidMove(source, destination)){
    error_code = ERROR_INVALID_PIECE_MOVE;
    printError(source, destination);
    return;
  }

  makeMove(source, destination);

  /* Checking if move puts own king in check and if so, undoing the move */
  if(isKingInCheck(player_turn)){
    undoMakeMove(source, destination);
    error_code = ERROR_OWN_KING_CHECK;
    printError(source, destination);
    return;
  }

  /* Getting to this stage means that the move is valid. If the move was
     castling, the first message below is printed and the second otherwise */
  if((position[destination]->getType() == Type::King) &&
     (dynamic_cast<King*>(position[destination])->getIsCastlingMove())){
    cout << position[destination]->printColour() << " has Castled ";
    if(source[0] < destination[0])
      cout << "kingside ";
    else
      cout << "queenside ";
    cout << "with King at " << destination << endl;
  }

  else{
    cout << position[destination]->printColour() << "'s "
         << position[destination]->printType() << " moves from " << source
	       << " to " << destination;
    if(captured_piece != nullptr){
      cout << " taking ";
      if(player_turn == Colour::White)
        cout << "Black's ";
      else
        cout << "White's ";
      cout << captured_piece->printType();
    }
    if((position[destination]->getType() == Type::Pawn) &&
       (dynamic_cast<Pawn*>(position[destination])->getIsEnPassantAttack())){
      cout << " using En Passant" << endl;
    }
    cout << endl;
  }

  //printBoard();

  /* Pawn Promotion: if the pawn went to the end of the chessboard, then it
     can be promoted */
  if((position[destination]->getType() == Type::Pawn) &&
     (destination[1] == RANK_MIN || destination[1] == RANK_MAX)){
    pawnPromotion(destination);
    //printBoard();
  }

  /* Setting first moves to false if the piece moved was a King, Rook or Pawn.
     For King, it sets Castling move to be false as well, which would have been
     set to true if the move was undertaken this turn. For Pawn, if it is has
     moved forward two steps in its first turn, it sets En Passant available
     to true so the opponent could perform an En Passant move on this piece in
     the next turn. It also changes the En Passant attack to false, which would
     have been set to true if the move was undertaken this turn */
  if(position[destination]->getType() == Type::King){
    dynamic_cast<King*>(position[destination])->setFirstMoveToFalse();
    dynamic_cast<King*>(position[destination])->setIsCastlingMove(false);
  }

  if(position[destination]->getType() == Type::Rook)
    dynamic_cast<Rook*>(position[destination])->setFirstMoveToFalse();

  if(position[destination]->getType() == Type::Pawn){
    if((dynamic_cast<Pawn*>(position[destination])->getIsFirstMove()) &&
       (abs(destination[1] - source[1]) == 2)){
      dynamic_cast<Pawn*>(position[destination])->setEnPassantAvailable(true);
    }
    dynamic_cast<Pawn*>(position[destination])->setFirstMoveToFalse();
    dynamic_cast<Pawn*>(position[destination])->setEnPassantAttack(false);
  }

  /* We can remove the captured piece from the game as the move was valid. This
     avoids us not deleting something that was created on the HEAP in our board
     destructor as this piece is not on the board anymore */
  if(captured_piece != nullptr){
    delete captured_piece;
    captured_piece = nullptr;
  }

  switchTurn();

  /* Sets en_passant_available to false, as an En Passant move can only be
     done the turn after an opposing pawn has taken two steps forward in its
     first move. A counter is triggered to be true once a pawn takes two
     steps forward in its first move and in the next turn, this is set false
     by the below */
  map<string, ChessPiece*>::iterator it;
  for(it = position.begin(); it != position.end(); ++it){
    if(!isEmptyPosition(it->first) && (it->second->getColour() == player_turn) &&
       (it->second->getType() == Type::Pawn)){
      dynamic_cast<Pawn*>(it->second)->setEnPassantAvailable(false);
    }
  }

  /* Checks if the other player is in check or check mate due to the move that
     was performed this turn */
  if(isKingInCheck(player_turn)){
    if(isAnyMovePossible(player_turn))
      cout << printPlayerTurn() << " is in check" << endl;
    else{
      cout << printPlayerTurn() << " is in checkmate" << endl;
      is_game_running = false;
      return;
    }
  }

  /* Checks if the other player in in stalemate due to the move that was
     performed this turn */
  if(!isAnyMovePossible(player_turn)){
    cout << printPlayerTurn() << " is in stalemate" << endl;
    is_game_running = false;
  }
}

bool ChessBoard::isValidInput(const string source, const string destination){
  if(!isCorrectFormat(source)){
    error_code = ERROR_INVALID_INPUT_SOURCE;
    return false;
  }

  if(!isCorrectFormat(destination)){
    error_code = ERROR_INVALID_INPUT_DESTINATION;
    return false;
  }

  if(!position[source]){
    error_code = ERROR_NO_PIECE_AT_SOURCE;
    return false;
  }

  if(position[source]->getColour() != player_turn){
    error_code = ERROR_WRONG_PLAYER;
    return false;
  }

  if(position[source] == position[destination]){
    error_code = ERROR_SOURCE_AND_DESTINATION_SAME;
    return false;
  }

  if(!isEmptyPosition(destination) &&
     !isEnemyPiece(destination, position[source]->getColour())){
    error_code = ERROR_FRIENDLY_PIECE_AT_DESTINATION;
    return false;
  }

  return true;
}

bool ChessBoard::isCorrectFormat(const string input){
  if(input.size() != 2)
    return false;

  if(input[0] < FILE_MIN || input[0] > FILE_MAX)
    return false;

  if(input[1] < RANK_MIN || input[1] > RANK_MAX)
    return false;

  return true;
}

void ChessBoard::makeMove(const string source, const string destination){
  if(position[source]->getType() == Type::Pawn){
    if(dynamic_cast<Pawn*>(position[source])->isEnPassant(source, destination)){
      makeEnPassantMove(source, destination);
      return;
    }
    else{
      makeNormalMove(source, destination);
      return;
    }
  }

  if(position[source]->getType() == Type::King){
    if(dynamic_cast<King*>(position[source])->isCastling(source, destination)){
      makeCastlingMove(source, destination);
      return;
    }
    else{
      makeNormalMove(source, destination);
      return;
    }
  }

  makeNormalMove(source, destination);
}

void ChessBoard::makeNormalMove(const string source, const string destination){
  captured_piece = position[destination];
  position[destination] = position[source];
  position[source] = nullptr;
}

void ChessBoard::makeEnPassantMove(const string source, const string destination){
  string enemy_pawn = destination;
  enemy_pawn[1] = source[1];
  captured_piece = position[enemy_pawn];
  position[destination] = position[source];
  position[source] = nullptr;
  position[enemy_pawn] = nullptr;
  dynamic_cast<Pawn*>(position[destination])->setEnPassantAttack(true);
}

void ChessBoard::makeCastlingMove(const string source, const string destination){
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

  position[destination] = position[source];
  position[source] = nullptr;
  position[rook_destination] = position[rook_source];
  position[rook_source] = nullptr;
  dynamic_cast<King*>(position[destination])->setIsCastlingMove(true);
}

void ChessBoard::undoMakeMove(const string source, const string destination){
  if(position[destination]->getType() == Type::Pawn){
    if(dynamic_cast<Pawn*>(position[destination])->getIsEnPassantAttack()){
      undoMakeEnPassantMove(source, destination);
      return;
    }
    else{
      undoMakeNormalMove(source, destination);
      return;
    }
  }

  if(position[destination]->getType() == Type::King){
    if(dynamic_cast<King*>(position[destination])->getIsCastlingMove()){
      undoMakeCastlingMove(source, destination);
      return;
    }
    else{
      undoMakeNormalMove(source, destination);
      return;
    }
  }

  undoMakeNormalMove(source, destination);
}

void ChessBoard::undoMakeNormalMove(const string source, const string destination){
  position[source] = position[destination];
  position[destination] = captured_piece;
  captured_piece = nullptr;
}


void ChessBoard::undoMakeEnPassantMove(const string source, const string destination){
  string enemy_pawn = destination;
  enemy_pawn[1] = source[1];
  position[source] = position[destination];
  position[enemy_pawn] = captured_piece;
  position[destination] = nullptr;
  captured_piece = nullptr;
  dynamic_cast<Pawn*>(position[source])->setEnPassantAttack(false);
}

void ChessBoard::undoMakeCastlingMove(const string source, const string destination){
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

  position[source] = position[destination];
  position[destination] = nullptr;
  position[rook_source] = position[rook_destination];
  position[rook_destination] = nullptr;
  dynamic_cast<King*>(position[source])->setIsCastlingMove(false);
}

void ChessBoard::findKingsPositions(){
  map<string, ChessPiece*>::iterator it;
  for(it = position.begin(); it != position.end(); ++it){
    if((it->second != nullptr) && (it->second->getType() == Type::King)){
      if(it->second->getColour() == Colour::White)
        white_king_position = it->first;
      else
        black_king_position = it->first;
    }
  }
}

bool ChessBoard::isKingInCheck(Colour colour){
  findKingsPositions();
  string destination;
  if(colour == Colour::White)
    destination = white_king_position;
  else
    destination = black_king_position;

  map<string, ChessPiece*>::iterator it;
  for(it = position.begin(); it != position.end(); ++it){
    if(!isEmptyPosition(it->first) && (it->second->getColour() != colour) &&
       it->second->isValidMove(it->first, destination)){
      return true;
    }
  }

  return false;
}

bool ChessBoard::isAnyMovePossible(Colour colour){
  map<string, ChessPiece*>::iterator possible_pieces;
  for(possible_pieces = position.begin(); possible_pieces != position.end(); ++possible_pieces){

    /* Checks for every possible piece on the board that is the same colour as
       the one passed in */
    if(!isEmptyPosition(possible_pieces->first) &&
       (possible_pieces->second->getColour() == colour)){
      string piece_to_stop_check = possible_pieces->first;
      map<string, ChessPiece*>::iterator possible_moves;
      for(possible_moves = position.begin(); possible_moves != position.end(); ++possible_moves){

        /* Checks all the valid moves that piece can make and makes the move */
        if((isValidInput(possible_pieces->first, possible_moves->first)) &&
           (possible_pieces->second->isValidMove(possible_pieces->first, possible_moves->first))){
          makeMove(possible_pieces->first, possible_moves->first);

          /* If the move results in the king not being in check, it undoes the
      	  move and returns true. Otherwise it undoes the move and tries with
      	  another move */
          if(!isKingInCheck(colour)){
            undoMakeMove(possible_pieces->first, possible_moves->first);
            return true;
          }

          undoMakeMove(possible_pieces->first, possible_moves->first);
        }
      }
    }
  }
  return false;
}

void ChessBoard::switchTurn(){
  if(player_turn == Colour::White)
    player_turn = Colour::Black;
  else
    player_turn = Colour::White;
}

void ChessBoard::pawnPromotion(const string destination){
  cout << "Pawn Promotion: Pawn at " << destination << " has been promoted!" << endl;
  cout << "What would you like your pawn to be promoted to? " << endl;
  cout << "Types available: \"Queen\", \"Bishop\", \"Knight\", \"Rook\"" << endl;

  string new_piece_type;
  bool piece_chosen = false;
  ChessPiece* temporary_piece = position[destination];
  position[destination] = nullptr;

  while(piece_chosen == false){
    cin >> new_piece_type;
    cout << endl;

    if(new_piece_type == "Queen"){
      position[destination] = new Queen(player_turn, this);
      piece_chosen = true;
    }

    else if(new_piece_type == "Bishop"){
      position[destination] = new Bishop(player_turn, this);
      piece_chosen = true;
    }

    else if(new_piece_type == "Knight"){
      position[destination] = new Knight(player_turn, this);
      piece_chosen = true;
    }

    else if(new_piece_type == "Rook"){
      position[destination] = new Rook(player_turn, this);
      piece_chosen = true;
    }

    else{
      cout << "You have entered an incorrect option!" << endl;
      cout << "Please enter one of the following: " << endl;
      cout << "\"Queen\", \"Bishop\", \"Knight\", \"Rook\"" << endl;
    }

  }

  delete temporary_piece;
  cout << "Pawn has been promoted to " << new_piece_type << "!" << endl;
}

ChessBoard::~ChessBoard(){
  deleteBoard();
}

void ChessBoard::deleteBoard(){
  map<string, ChessPiece*>::iterator it;
  for(it = position.begin(); it != position.end(); ++it){
    if(!isEmptyPosition(it->first))
      delete it->second;
  }
}

void ChessBoard::resetBoard(){
  deleteBoard();
  initialiseBoard();
}

void ChessBoard::printBoard(){
  cout << "    ";
  for(char file = FILE_MIN; file <= FILE_MAX; file++)
    cout << file << "   ";
  cout << endl;
  for(char rank = RANK_MAX; rank >= RANK_MIN; rank--){
    cout << "  +---+---+---+---+---+---+---+---+" << endl;
    cout << rank << " ";
    for(char file = FILE_MIN; file <= FILE_MAX; file++){
      cout << "| ";
      if(position[string(1, file) + string(1, rank)] != nullptr)
        cout << position[string(1, file) + string(1, rank)]->getSymbol() << " ";
      else
        cout << "  ";
    }
    cout << "| " << endl;
  }
  cout << "  +---+---+---+---+---+---+---+---+" << endl;
}

string ChessBoard::printPlayerTurn(){
  switch(player_turn){
    case Colour::White:
      return "White";

    case Colour::Black:
      return "Black";

    default:
      return "NULL";
  }
}

void ChessBoard::printError(const string source, const string destination){
  switch(error_code){
    case ERROR_GAME_NOT_RUNNING:
      cerr << "Game has already ended! Please reset the board to start a new game." << endl;
      break;

    case ERROR_INVALID_INPUT_SOURCE:
      cerr << "Invalid source position: " << source << ". Please enter file between A and "
           << "H as a capital letter and rank between 1 and 8." << endl;
      break;

    case ERROR_INVALID_INPUT_DESTINATION:
      cerr << "Invalid destination position: " << destination << ". Please enter file "
           << "between A and H as a capital letter and rank between 1 and 8." << endl;
      break;

    case ERROR_NO_PIECE_AT_SOURCE:
      cerr << "There is no piece at position " << source << "!" << endl;
      break;

    case ERROR_WRONG_PLAYER:
      cerr << "It is not " << position[source]->printColour() << "'s turn to move!" << endl;
      break;

    case ERROR_SOURCE_AND_DESTINATION_SAME:
      cerr << "Entered the source position " << source << " and destination position "
           << destination << ". They cannot be the same!" << endl;
      break;

    case ERROR_FRIENDLY_PIECE_AT_DESTINATION:
      cerr << "Attempted to move " << position[source]->printColour() <<"'s "
           << position[source]->printType() << " from " << source
           << " to " << destination << " but the destination contains a friendly "
           << position[destination]->printType() << "!" << endl;
      break;

    case ERROR_INVALID_PIECE_MOVE:
      cerr << position[source]->printColour() <<"'s " << position[source]->printType()
           << " cannot move to " << destination << "!" << endl;
      if(position[source]->getType() == Type::King){
        cerr << "For castling, please enter the King's source and destination squares"
             << endl;
      }
      break;

    case ERROR_OWN_KING_CHECK:
      cerr << "Moving " << position[source]->printColour() << "'s "
           << position[source]->printType() << " from " << source << " to "
           << destination << " puts own King in check!" << endl;
      break;

    default: //do nothing
      break;
  }
}

bool ChessBoard::isEmptyPosition(const string input){
  if(position[input] == nullptr)
    return true;
  return false;
}

bool ChessBoard::isEnemyPiece(const string destination, Colour colour){
  if(!isEmptyPosition(destination) && (position[destination]->getColour() != colour))
    return true;
  return false;
}

bool ChessBoard::isCastlingValid(const string king_source, const string king_destination,
				                         const string rook_source, const string rook_destination){

  /* If the square where the rook is meant to be is empty or it doesn't
     contain a rook or if it is a rook, which has already moved in the
     game, it returns false. No need to check colour as well since if
     it is the first move then that piece would be the same colour as
     the king, which hasn't moved either */
  if(isEmptyPosition(rook_source) ||
     (position[rook_source]->getType() != Type::Rook) ||
     (!dynamic_cast<Rook*>(position[rook_source])->getIsFirstMove())){
    return false;
  }

  /* Is the king currently in check in its current position */
  if(isKingInCheck(player_turn))
    return false;

  int file_change = 0;
  if(king_source[0] < king_destination[0])
    file_change = 1;
  else
    file_change = -1;

  string temporary_king = king_source;
  temporary_king[0] += file_change;
  position[temporary_king] = position[king_source];
  position[king_source] = nullptr;

  /* Will the king be in check if it moves one square in the castling move */
  if(isKingInCheck(player_turn)){
    position[king_source] = position[temporary_king];
    position[temporary_king] = nullptr;
    return false;
  }

  position[king_destination] = position[temporary_king];
  position[temporary_king] = nullptr;

  /* Will the king be in check if it moves to the destination square in the
     castling move */
  if(isKingInCheck(player_turn)){
    position[king_source] = position[king_destination];
    position[king_destination] = nullptr;
    return false;
  }

  /* If it reaches here, then the king was not in check at the start, during or
     end of castling. Return everything back to the original state before this
     function was undertaken */
  position[king_source] = position[king_destination];
  position[king_destination] = nullptr;
  return true;
}

bool ChessBoard::isEnPassantValid(const string enemy_pawn_source, Colour colour){
  if((position[enemy_pawn_source]->getType() == Type::Pawn) &&
     (position[enemy_pawn_source]->getColour() != colour) &&
     (dynamic_cast<Pawn*>(position[enemy_pawn_source])->getIsEnPassantAvailable())){
    return true;
  }
  return false;
}
