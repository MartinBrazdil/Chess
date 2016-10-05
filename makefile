PARAMS = -g -Wall -pedantic -std=c++11 -pthread -O3

all : chess

chess : argparser.o pieces.o bitboard.o chess.o lookuptables.o chessAI.o abstractthread.o
	g++ ${PARAMS} argparser.o pieces.o bitboard.o lookuptables.o chessAI.o abstractthread.o chess.o -o chess

argparser.o : argparser.cpp argparser.h
	g++ ${PARAMS} argparser.cpp -c

lookuptables.o : lookuptables.cpp lookuptables.h
	g++ ${PARAMS} lookuptables.cpp -c

chessAI.o : chessAI.cpp chessAI.h pieces.h abstractthread.h
	g++ ${PARAMS} chessAI.cpp -c
	
chess.o : chess.cpp chess.h
	g++ ${PARAMS} chess.cpp -c

bitboard.o : bitboard.cpp bitboard.h
	g++ ${PARAMS} bitboard.cpp -c

abstractthread.o : abstractthread.cpp abstractthread.h
	g++ ${PARAMS} abstractthread.cpp -c

pieces.o : pieces.cpp
	g++ ${PARAMS} pieces.cpp -c

clean :
	rm -f *.o chess
	make

run : 
	rm -f *.o chess
	make
	./chess
