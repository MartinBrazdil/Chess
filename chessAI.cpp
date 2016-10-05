#include <map>
#include <string>
#include <iostream>
#include <algorithm>
#include <algorithm>

using namespace std;

#include "constants.h"
#include "pieces.h"
#include "bitboard.h"
#include "argparser.h"

#include "chessAI.h"

pthread_mutex_t mutex;
int moveCounter = 0;

ChessAI::ChessAI ( ) {
    if(ArgParser::setArgs.find("depth") != ArgParser::setArgs.end()) {
        depth = ArgParser::setArgs.find("depth")->second;
    } else { depth = 5; }
    if(ArgParser::setArgs.find("info") != ArgParser::setArgs.end()) {
        info = ArgParser::setArgs.find("info")->second;
    } else { info = 0; }
}

ChessAI::ChessAI ( int depth ) {
    this->depth = depth;
    if(ArgParser::setArgs.find("info") != ArgParser::setArgs.end()) {
        info = ArgParser::setArgs.find("info")->second;
    } else { info = 0; }
}

void * ChessAI::ThreadWorker ( void * arg ) {
    double score = AIChooseNextMove(data.side, data.plieCntr, data.enemyMove, data.currentBoard, data.score, data.id, data.castling);
    return (void*)(new pair<double,Move>(score,data.enemyMove));
}

uint64_t ChessAI::AllAttacks ( Piece ** Pieces, Values & CurrentBoard ) {
    uint64_t attacks = 0;
    for(int iterator = 0; iterator < 6; iterator++) {
        int pieceCount = Pieces[iterator]->Split(Pieces[iterator]->position, Pieces[iterator]->pieces);
        for(int i = 0; i < pieceCount; i++) {
            attacks |= Pieces[iterator]->Attacks(CurrentBoard.AllPieces, CurrentBoard.BPieces, CurrentBoard.WPieces, CurrentBoard.Board, i);
        }
    }
    return attacks;
}

bool ChessAI::WKingCheckControl ( Values currentBoard, Move chosenMove ) {
    BitBoard::MovePiece(chosenMove.piece->GetPiece(currentBoard), chosenMove.from, chosenMove.to, currentBoard.WPieces, currentBoard);
    Piece * Pieces[6] = { &(currentBoard.BPawns), &(currentBoard.BKnights), &(currentBoard.BKing),
                          &(currentBoard.BBishops), &(currentBoard.BRooks), &(currentBoard.BQueen) };
    uint64_t attacks = AllAttacks(Pieces, currentBoard);
    if((attacks & currentBoard.WKing.position) == currentBoard.WKing.position) {
        return true;
    }
    return false;
}

bool ChessAI::BKingCheckControl ( Values currentBoard, Move chosenMove ) {
    BitBoard::MovePiece(chosenMove.piece->GetPiece(currentBoard), chosenMove.from, chosenMove.to, currentBoard.BPieces, currentBoard);
    Piece * Pieces[6] = { &(currentBoard.WPawns), &(currentBoard.WKnights), &(currentBoard.WKing),
                          &(currentBoard.WBishops), &(currentBoard.WRooks), &(currentBoard.WQueen) };
    uint64_t attacks = AllAttacks(Pieces, currentBoard);
    if((attacks & currentBoard.BKing.position) == currentBoard.BKing.position) {
        return true;
    }
    return false;
}

double ChessAI::AIChooseNextMove ( bool side, int plieCntr, Move enemyMove, Values CurrentBoard, double score, uint64_t id, int castling ) {
    uint64_t allMoves, numOfChilds = 0;
    uint64_t wattacks, battacks;
    double thisScore = 0;
    int pieceCount, moveCount;
    Piece * BPieces[6], * WPieces[6];
    Move chosenMove;

    if(side && enemyMove.piece != NULL) {
        if(castling >= 0) {
            score += BitBoard::MovePiece(castling, CurrentBoard.BPieces, CurrentBoard);
        } else {
            score += BitBoard::MovePiece(enemyMove.piece->GetPiece(CurrentBoard), enemyMove.from, enemyMove.to, CurrentBoard.BPieces, CurrentBoard);
        }
    } else if(enemyMove.piece != NULL) {
        if(castling >= 0) {
            score += BitBoard::MovePiece(castling, CurrentBoard.WPieces, CurrentBoard);
        } else {
            score -= 2*(BitBoard::MovePiece(enemyMove.piece->GetPiece(CurrentBoard), enemyMove.from, enemyMove.to, CurrentBoard.WPieces, CurrentBoard));
        }
    }

    if(plieCntr >= depth) return score;

    BPieces[0]=&(CurrentBoard.BPawns), BPieces[1]=&(CurrentBoard.BKnights), BPieces[2]=&(CurrentBoard.BKing),
    BPieces[3]=&(CurrentBoard.BBishops), BPieces[4]=&(CurrentBoard.BRooks), BPieces[5]=&(CurrentBoard.BQueen);

    WPieces[0]=&(CurrentBoard.WPawns), WPieces[1]=&(CurrentBoard.WKnights), WPieces[2]=&(CurrentBoard.WKing),
    WPieces[3]=&(CurrentBoard.WBishops), WPieces[4]=&(CurrentBoard.WRooks), WPieces[5]=&(CurrentBoard.WQueen);          

    if(side == 0) /*CERNI*/ {

        for(int iterator = 0; iterator < 6; iterator++) {
            pieceCount = BPieces[iterator]->Split(BPieces[iterator]->position, BPieces[iterator]->pieces);
            for(int i = 0; i < pieceCount; i++) {
                allMoves   = BPieces[iterator]->AllMoves(CurrentBoard.AllPieces, CurrentBoard.BPieces, CurrentBoard.WPieces, CurrentBoard.Board, i);
                moveCount  = BPieces[iterator]->Split(allMoves, BPieces[iterator]->moves);
                for(int y = 0; y < moveCount; y++) {
                    chosenMove.Initialize(BPieces[iterator], BPieces[iterator]->pieces[i], BPieces[iterator]->moves[y], CurrentBoard.BPieces);
                    if(BKingCheckControl(CurrentBoard, chosenMove)) {
                        continue;
                    }
                    if(info == 1) {
                        pthread_mutex_lock(&mutex);
                        moveCounter++;
                        pthread_mutex_unlock(&mutex);
                    }
                    if(id == 0) {
                        numOfChilds++;
                        data.Initialize(1, plieCntr+1, chosenMove, CurrentBoard, score, numOfChilds, -1);
                        Start();
                    } else {
                        thisScore += AIChooseNextMove(1, plieCntr+1, chosenMove, CurrentBoard, score, id);
                    }
                }
            }
        }

        if(CurrentBoard.BQueensideCastling) {
            if(CurrentBoard.BRooks.position & A1) {
                if(!(CurrentBoard.Board & (B8 | C8 | D8))) {
                    wattacks = AllAttacks(WPieces, CurrentBoard);
                    if(!(wattacks & (CurrentBoard.BKing.position | D8 | C8))) {
                        CurrentBoard.BQueensideCastling = false;
                        CurrentBoard.BKingsideCastling = false;
                        chosenMove.Initialize(&CurrentBoard.BKing,0,0,0,0);
                        if(id == 0) {
                            if(info == 1) {
                                pthread_mutex_lock(&mutex);
                                moveCounter++;
                                pthread_mutex_unlock(&mutex);
                            }
                            numOfChilds++;
                            data.Initialize(1, plieCntr+1, chosenMove, CurrentBoard, score, numOfChilds, 0);
                            Start();
                        } else {
                            thisScore += AIChooseNextMove(1, plieCntr+1, chosenMove, CurrentBoard, score, id, 0);
                        }
                        CurrentBoard.BKingsideCastling = true;
                    }
                }
            }
        }
        if(CurrentBoard.BKingsideCastling) {
            if(CurrentBoard.BRooks.position & H8) {
                if(!(CurrentBoard.Board & (F8 | G8))) {
                    wattacks = AllAttacks(WPieces, CurrentBoard);
                    if(!(wattacks & (CurrentBoard.BKing.position | F8 | G8))) {
                        //kingside castling
                        CurrentBoard.BQueensideCastling = false;
                        CurrentBoard.BKingsideCastling = false;
                        if(info == 1) {
                            pthread_mutex_lock(&mutex);
                            moveCounter++;
                            pthread_mutex_unlock(&mutex);
                        }
                        chosenMove.Initialize(&CurrentBoard.BKing,0,0,0,1);
                        if(id == 0) {
                            numOfChilds++;
                            data.Initialize(1, plieCntr+1, chosenMove, CurrentBoard, score, numOfChilds, 1);
                            Start();
                        } else {
                            thisScore += AIChooseNextMove(1, plieCntr+1, chosenMove, CurrentBoard, score, id, 1);
                        }
                        CurrentBoard.BQueensideCastling = true;
                    }
                }
            }
        }

    } else if(side == 1)/*BILI*/ {

        for(int iterator = 0; iterator < 6; iterator++) {
            pieceCount = WPieces[iterator]->Split(WPieces[iterator]->position, WPieces[iterator]->pieces);
            for(int i = 0; i < pieceCount; i++) {
                allMoves   = WPieces[iterator]->AllMoves(CurrentBoard.AllPieces, CurrentBoard.BPieces, CurrentBoard.WPieces, CurrentBoard.Board, i);
                moveCount  = WPieces[iterator]->Split(allMoves, WPieces[iterator]->moves);
                for(int y = 0; y < moveCount; y++) {
                    chosenMove.Initialize(WPieces[iterator], WPieces[iterator]->pieces[i], WPieces[iterator]->moves[y], CurrentBoard.WPieces);
                    if(WKingCheckControl(CurrentBoard, chosenMove)) {
                        continue;
                    }
                    if(info == 1) {
                        pthread_mutex_lock(&mutex);
                        moveCounter++;
                        pthread_mutex_unlock(&mutex);
                    }
                    if(id == 0) {
                        numOfChilds++;
                        data.Initialize(0, plieCntr+1, chosenMove, CurrentBoard, score, numOfChilds, -1);
                        Start();
                    } else {
                        thisScore += AIChooseNextMove(0, plieCntr+1, chosenMove, CurrentBoard, score, id);
                    }
                }
            }
        }

        if(CurrentBoard.WQueensideCastling) {
            if(CurrentBoard.WRooks.position & A1) {
                if(!(CurrentBoard.Board & (B1 | C1 | D1))) {
                    battacks = AllAttacks(BPieces, CurrentBoard);
                    if(!(battacks & (CurrentBoard.WKing.position | D1 | C1))) {
                        //queenside castling
                        CurrentBoard.WQueensideCastling = false;
                        CurrentBoard.WKingsideCastling = false;
                        if(info == 1) {
                            pthread_mutex_lock(&mutex);
                            moveCounter++;
                            pthread_mutex_unlock(&mutex);
                        }
                        chosenMove.Initialize(&CurrentBoard.WKing,0,0,0,0);
                        if(id == 0) {
                            numOfChilds++;
                            data.Initialize(0, plieCntr+1, chosenMove, CurrentBoard, score, numOfChilds, 0);
                            Start();
                        } else {
                            thisScore += AIChooseNextMove(0, plieCntr+1, chosenMove, CurrentBoard, score, id, 0);
                        }
                        CurrentBoard.WKingsideCastling = true;
                    }
                }
            }
        }
        if(CurrentBoard.WKingsideCastling) {
            if(CurrentBoard.WRooks.position & H1) {
                if(!(CurrentBoard.Board & (F1 | G1))) {
                    battacks = AllAttacks(BPieces, CurrentBoard);
                    if(!(battacks & (CurrentBoard.WKing.position | F1 | G1))) {
                        //kingside castling
                        CurrentBoard.WQueensideCastling = false;
                        CurrentBoard.WKingsideCastling = false;
                        if(info == 1) {
                            pthread_mutex_lock(&mutex);
                            moveCounter++;
                            pthread_mutex_unlock(&mutex);
                        }
                        chosenMove.Initialize(&CurrentBoard.WKing,0,0,0,1);
                        if(id == 0) {
                            numOfChilds++;
                            data.Initialize(0, plieCntr+1, chosenMove, CurrentBoard, score, numOfChilds, 1);
                            Start();
                        } else {
                            thisScore += AIChooseNextMove(0, plieCntr+1, chosenMove, CurrentBoard, score, id, 1); 
                        }
                        CurrentBoard.WQueensideCastling = true;
                    }
                }
            }
        }
    }

    if(id == 0) {
        void ** threadResults = Wait();
        for(int i = 0; i < threadCounter; i++) {
            if(info == 1) {
                cout << "Thread[" << i << "] score: " << (*((pair<double,Move>*)threadResults[i])).first << endl;
            }
            results.insert(*((pair<double,Move>*)threadResults[i]));
        }
        if(info == 1) {
            cout << "Number of probed moves: " << moveCounter << endl;
        }
        delete [] threadResults;
    }

    return score+thisScore;
}
