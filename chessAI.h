#ifndef CHESSAI
#define CHESSAI

#include "pieces.h"
#include "abstractthread.h"

class ChessAI : public AbstractThread {

private:

  struct Data {

    void Initialize(bool side, int plieCntr, Move enemyMove, Values currentBoard, double score, uint64_t id, int castling) {
      this->side               = side;
      this->plieCntr           = plieCntr;
      this->enemyMove.piece    = enemyMove.piece->Copy();
      this->enemyMove.from     = enemyMove.from;
      this->enemyMove.to       = enemyMove.to;
      this->enemyMove.side     = enemyMove.side;
      this->enemyMove.castling = enemyMove.castling;
      this->currentBoard       = currentBoard;
      this->score              = score;
      this->id                 = id;
      this->castling           = castling;
    }

                ~Data() { }

    bool        side;
    int         plieCntr;
    Move        enemyMove;
    Values      currentBoard;
    double      score;
    uint64_t    id;
    int         castling;
  };

  Data                            data;
  int                             depth;
  int                             info;

  virtual AbstractThread *        Copy                    ( void ) const { return new ChessAI(*this); }
  virtual void *                  ThreadWorker            ( void * arg );
  static uint64_t                 AllAttacks              ( Piece ** PIECES, Values & CurrentBoard );
  bool                            WKingCheckControl       ( Values currentBoard, Move chosenMove );
  bool                            BKingCheckControl       ( Values currentBoard, Move chosenMove );
  
public:


  multimap<double,Move>           results;

                                  ChessAI                 ( );
                                  ChessAI                 ( int depth );
  virtual                         ~ChessAI                ( ) { }
  double                          AIChooseNextMove        ( bool side, int plieCntr, Move enemyMove, Values currentBoard, double score, uint64_t id, int castling = -1 );
};

#endif
