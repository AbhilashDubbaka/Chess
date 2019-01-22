OBJ = ChessMain.o ChessBoard.o ChessPiece.o King.o Queen.o Bishop.o Knight.o Rook.o Pawn.o
CXX = g++
CXXFLAGS = -Wall -g -MMD
EXE = chess

$(EXE):$(OBJ)
	$(CXX) $^ -o $@

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $<

-include $(OBJ:.o=.d)

.PHONY: clean

clean:
	rm -f $(OBJ) $(EXE) $(OBJ:.o=.d)
