#ifndef HELPER_H
#define HELPER_H

const char FILE_MIN = 'A';
const char FILE_MAX = 'H';
const char RANK_MIN = '1';
const char RANK_MAX = '8';

/* A White pawn can only use the En Passant move once 
   it is on rank 5 */
const char EN_PASSANT_WHITE_ATTACK_RANK = '5';

/* A Black pawn can only use the En Passant move once 
   it is on rank 4 */
const char EN_PASSANT_BLACK_ATTACK_RANK = '4';

enum class Colour {White, Black};
enum class Type {King, Queen, Bishop, Knight, Rook, Pawn};

#endif
