#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "Errors.h"
#include "Helper.h"
#include <map>

/* Forward declaration of a ChessPiece */
class ChessPiece;

class ChessBoard{
 private:
   /* A mapping of every single position on a chessboard to a piece or null */
  std::map<std::string, ChessPiece*> position;

  /* Checks if the game is still active or if it has already finished */
  bool is_game_running;

  /* Says which player's turn it is */
  Colour player_turn;

  /* Holds the location of the white king position on the chessboard */
  std::string white_king_position;

  /* Holds the location of the white black position on the chessboard */
  std::string black_king_position;

  /* Holds the location of the captured piece in that turn */
  ChessPiece* captured_piece;

  /* Used to pass error codes between member functions to print the appropriate
     error message */
  int error_code;

  /* Initialises the chessboard at the start of a game. It set is_game_running
     to true, player_turn to White, white_king_position to E1, black_king_position
     to E8, captured piece points to null, error_code to SUCCESS, and sets all
     the pieces in the correct positions on the chessboard */
  void initialiseBoard();

  /* Checks if the user input is valid using source and destination string inputs
     from the user. It returns true if the source and destination squares in the
     right format and within the bounds of the chessboard, and if there is a
     piece at the source position, and if the source and destination are not the
     same position, and if the destination is not empty, then it contains a
     piece of the opposite colour as the source square. It returns if any one of
     the conditions is false */
  bool isValidInput(const std::string source, const std::string destination);

  /* Checks the user input has the correct format i.e. is only two characters in
     the format of file and rank i.e. A1. It checks if the first character is
     between A and H (inclusive) and if the second character is between 1 and 8
     (inclusive) It takes in the string input from the user and returns true if
     it is in the correct format and false otherwise */
  bool isCorrectFormat(const std::string input);

  /* This makes the chess move on the chessboard. Using the source and destination
     inputs, it will move the source piece to the destination. Any enemy pieces
     in that position are stored on the captured_piece pointer. If the source is
     a pawn, it will check if the move is an En Passant or a normal move and it
     will do the corresponding move. If the source is a king, it will check if
     the move is castling or a normal move and it will do the corresponding move.
     If it is any other piece, it will do the normal move */
  void makeMove(const std::string source, const std::string destination);

  /* This is a normal chess move. It takes in the source and destination string
     inputs and moves the piece at the source to the destination square. If there
     is a enemy piece at the destination square, it moves it into the captured_piece
     pointer */
  void makeNormalMove(const std::string source, const std::string destination);

  /* This takes in the source and destination strings and does an enpassant move,
     so it moves the pawn diagonally forward and captures the enemy piece that
     was beside the pawn when it was at the source position. It stores the captured
     piece into the captured_piece pointer. It sets is_en_passant_attack to true,
     which is used if there is a need to undo the move */
  void makeEnPassantMove(const std::string source, const std::string destination);

  /* Makes the castling move i.e. moves the King from the source to the destination,
     which are taken as inputs. It will find the castling rook and move it to its
     corresponding position */
  void makeCastlingMove(const std::string source, const std::string destination);

  /* This undoes the move made in make move. Using the source and destination
     inputs, it will move the destination piece back to the source. Any enemy pieces
     in the captured_piece pointer will be placed back on the destination square.
     If the destination square now has a pawn, it will check if the move was an
     En Passant or a normal move and it will undo the corresponding move. If the
     destination is now a king, it will check if the move was castling or a
     normal move and it will undo the corresponding move. If it is any other
     piece, it will undo the normal move */
  void undoMakeMove(const std::string source, const std::string destination);

  /* This undoes the normal move. It takes in the source and destination string
     inputs and moves the piece at the destination back to the destination square.
     If there is an enemy piece that the captured_piece pointer is pointing to,
     it moves it back to the destination square */
  void undoMakeNormalMove(const std::string source, const std::string destination);

  /* This undoes the En Passant move. This takes in the source and destination
     strings and moves the pawn diagonally backward and puts back the enemy piece
     that was beside the pawn when it was at the source position. It sets
     is_en_passant_attack back to false */
  void undoMakeEnPassantMove(const std::string source, const std::string destination);

  /* Undoes the castling move i.e. moves the King from the destination to the source,
     which are taken as inputs. It will find the castling rook and move it back
     to its original position */
  void undoMakeCastlingMove(const std::string source, const std::string destination);

  /* This finds the kings positions' on the chessboard and stores into the
     white_king_position and black_king_position data members */
  void findKingsPositions();

  /* This takes in a colour i.e. White or Black and checks if the King of that
     colour is in check. It looks through all the enemy pieces on the chessboard
     and checks if any of them can legally move into King's position i.e. capture
     it in the next turn. This returns true if an enemy piece can do this and
     false if not */
  bool isKingInCheck(Colour colour);

  /* Takes in a colour and checks if there are any valid moves that colour player
     can make. It returns true as soon as it find one possible move that player
     can make and false if there is no move that player can make */
  bool isAnyMovePossible(Colour colour);

  /* Switched player's turn once their turn has ended */
  void switchTurn();

  /* If a pawn reaches the end of the board, then it can be promoted to a Queen,
     Bishop, Knight or Rook, which replaces the existing pawn. This function takes
     in the destination of the pawn and if it is at the end of the board, it gives
     an option to the user to promote it */
  void pawnPromotion(const std::string destination);

  /* Prints out the chessboard for the user to see the state of the board */
  void printBoard();

  /* Prints out whose turn it is */
  std::string printPlayerTurn();

  /* Prints out the correct error message when the user inputs an invalid inputs
     or an invalid move */
  void printError(const std::string source, const std::string destination);

  /* Deletes all the pieces on the board */
  void deleteBoard();

 public:
   /* Constructor of the chessboard - calls the initialise board function */
  ChessBoard();

  /* Destructor of the chessboard - calls the delete board function */
  ~ChessBoard();

  /* Submits the move proposed by the user and takes in a strings for source and
     destionation squares. This function checks for the validity of the move and
     if it is, it will make the move and check if the move has put yourself in
     check. If not, it will check if the move has put the other player in check,
     checkmate or stalemate. It will print out a corresponding error message if
     at any point it notices as issue with the move proposed */
  void submitMove(const std::string source, const std::string destination);

  /* This resets the board by calling the delete board and then initialises the
     board */
  void resetBoard();

  /* Using a string input of a position on the board, it checks if that position
     has a piece and returns false  or if its empty and return true */
  bool isEmptyPosition(const std::string input);

  /* This takes in a string, which refers to the destination square and a colour.
     If there is a piece of the opposite colour in the destination, it returns
     true and false otherwise */
  bool isEnemyPiece(const std::string destination, Colour colour);

  /* Checks that the square where the rook is meant to be is not empty and it
     contains a rook, which hasn't moved already in the game, then it checks
     if the king is in check, will be in check during its move to its position
     or will be in check at the destination position. It takes in two strings
     for the source and destination squares and returns false if any of those
     conditions fail and true if not. This is used in checking for a valid move
     for the king */
  bool isCastlingValid(const std::string king_source, const std::string king_destination,
		                   const std::string rook_source, const std::string rook_destination);

  /* Check that the there is an enemy pawn where it is supposed to be for an
     En Passant to take place and that it can still be captured via an En
     Passant move. It takes in two strings for the source square and the colour
     of the pawn trying to do the En Passant attack so a friendly piece is not
     attacked. It returns false if any of those conditions fail and true if not.
     This is used in checking for a valid move for the pawn */
  bool isEnPassantValid(const std::string enemy_pawn_source, Colour colour);

};

#endif
