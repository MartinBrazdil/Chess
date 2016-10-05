#ifndef PIECES
#define PIECES

#include <map>

struct Values;

void GlobalLookupTablesInit ( );

/********************************************************************************************************************/

class Piece {

protected:

  int                               GetCoordinate         ( const uint64_t Position ) const;

public:

    uint64_t                          pieces[8];
    uint64_t                          moves[28];
    uint64_t                          position;
    uint64_t                          validMoves;

                                      Piece                 ( );
    virtual                           ~Piece                ( ) { }
    virtual uint64_t                  AllMoves              ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) = 0;
    virtual uint64_t                  Attacks               ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) = 0;
    static int                        Split                 ( uint64_t ToSplit, uint64_t * Pieces );
    virtual uint64_t &                GetPiece              ( Values &ChosenBoard ) = 0;
    virtual Piece *                   Copy                  ( ) = 0;

};

/********************************************************************************************************************/

class PawnsW : public Piece {

protected:

    uint64_t                          WPawnMoves            ( uint64_t AllPieces, int n );
    uint64_t                          WPawnAttacks          ( int n );

public:

    virtual                           ~PawnsW               ( ) { }
    virtual uint64_t                  AllMoves              ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n );
    virtual uint64_t                  Attacks               ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) { return WPawnAttacks(n); }
    uint64_t &                        GetPiece              ( Values &ChosenBoard );
    virtual Piece *                   Copy                  ( ) { return new PawnsW(*this); }

};

class PawnsB : public Piece {

protected:

    uint64_t                          BPawnMoves            ( uint64_t AllPieces, int n );
    uint64_t                          BPawnAttacks          ( int n );

public:

    virtual                           ~PawnsB               ( ) { }
    virtual uint64_t                  AllMoves              ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n );
    virtual uint64_t                  Attacks               ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) { return BPawnAttacks(n); }
    uint64_t &                        GetPiece              ( Values &ChosenBoard );
    virtual Piece *                   Copy                  ( ) { return new PawnsB(*this); }

};

/********************************************************************************************************************/

class KnightsW : public Piece {

public:

    virtual                           ~KnightsW             ( ) { }
    virtual uint64_t                  AllMoves              ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n );
    virtual uint64_t                  Attacks               ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) { return AllMoves(AllPieces, BPieces, WPieces, Board, n); }
    uint64_t &                        GetPiece              ( Values &ChosenBoard );
    virtual Piece *                   Copy                  ( ) { return new KnightsW(*this); }

};

class KnightsB : public Piece {

public:

    virtual                           ~KnightsB             ( ) { }
    virtual uint64_t                  AllMoves              ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n );
    virtual uint64_t                  Attacks               ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) { return AllMoves(AllPieces, BPieces, WPieces, Board, n); }
    uint64_t &                        GetPiece              ( Values &ChosenBoard );
    virtual Piece *                   Copy                  ( ) { return new KnightsB(*this); }

};

/********************************************************************************************************************/

class BishopsW : public Piece {

public: 

    virtual                           ~BishopsW             ( ) { }
    virtual uint64_t                  AllMoves              ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n );
    virtual uint64_t                  Attacks               ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) { return AllMoves(AllPieces, BPieces, WPieces, Board, n); }
    uint64_t &                        GetPiece              ( Values &ChosenBoard );
    virtual Piece *                   Copy                  ( ) { return new BishopsW(*this); }

};

class BishopsB : public Piece {

public: 

    virtual                           ~BishopsB             ( ) { }
    virtual uint64_t                  AllMoves              ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n );
    virtual uint64_t                  Attacks               ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) { return AllMoves(AllPieces, BPieces, WPieces, Board, n); }
    uint64_t &                        GetPiece              ( Values &ChosenBoard );
    virtual Piece *                   Copy                  ( ) { return new BishopsB(*this); }

};

/********************************************************************************************************************/

class RooksW : public Piece {

public:

    virtual                           ~RooksW               ( ) { }
    virtual uint64_t                  AllMoves              ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n );
    virtual uint64_t                  Attacks               ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) { return AllMoves(AllPieces, BPieces, WPieces, Board, n); }
    uint64_t &                        GetPiece              ( Values &ChosenBoard );
    virtual Piece *                   Copy                  ( ) { return new RooksW(*this); }

};

class RooksB : public Piece {

public:

    virtual                           ~RooksB               ( ) { }
    virtual uint64_t                  AllMoves              ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n );
    virtual uint64_t                  Attacks               ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) { return AllMoves(AllPieces, BPieces, WPieces, Board, n); }
    uint64_t &                        GetPiece              ( Values &ChosenBoard );
    virtual Piece *                   Copy                  ( ) { return new RooksB(*this); }

};

/********************************************************************************************************************/

class QueenW : public Piece {

public:
  
    virtual                           ~QueenW               ( ) { }
    virtual uint64_t                  AllMoves              ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n );
    virtual uint64_t                  Attacks               ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) { return AllMoves(AllPieces, BPieces, WPieces, Board, n); }
    uint64_t &                        GetPiece              ( Values &ChosenBoard );
    virtual Piece *                   Copy                  ( ) { return new QueenW(*this); }

};

class QueenB : public Piece {

public:

    virtual                           ~QueenB               ( ) { }
    virtual uint64_t                  AllMoves              ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n );
    virtual uint64_t                  Attacks               ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) { return AllMoves(AllPieces, BPieces, WPieces, Board, n); }
    uint64_t &                        GetPiece              ( Values &ChosenBoard );
    virtual Piece *                   Copy                  ( ) { return new QueenB(*this); }

};

/********************************************************************************************************************/

class KingW : public Piece {

public:
  
    virtual                           ~KingW                ( ) { }
    virtual uint64_t                  AllMoves              ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n );
    virtual uint64_t                  Attacks               ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) { return AllMoves(AllPieces, BPieces, WPieces, Board, n); }
    uint64_t &                        GetPiece              ( Values &ChosenBoard );
    virtual Piece *                   Copy                  ( ) { return new KingW(*this); }

};

class KingB : public Piece {

public:
  
    virtual                           ~KingB                ( ) { }
    virtual uint64_t                  AllMoves              ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n );
    virtual uint64_t                  Attacks               ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) { return AllMoves(AllPieces, BPieces, WPieces, Board, n); }
    uint64_t &                        GetPiece              ( Values &ChosenBoard );
    virtual Piece *                   Copy                  ( ) { return new KingB(*this); }

};

/********************************************************************************************************************/

struct Values {

    bool      BQueensideCastling;
    bool      BKingsideCastling;
    bool      WQueensideCastling;
    bool      WKingsideCastling;

    uint64_t  AllPieces;
    uint64_t  WPieces;
    uint64_t  BPieces;

    PawnsW    WPawns;
    KnightsW  WKnights;
    BishopsW  WBishops;
    RooksW    WRooks;
    QueenW    WQueen;
    KingW     WKing;

    PawnsB    BPawns;
    KnightsB  BKnights;
    BishopsB  BBishops;
    RooksB    BRooks;
    QueenB    BQueen;
    KingB     BKing;

    uint64_t  Board;
};

/********************************************************************************************************************/

struct Move {
    Move() {
        this->piece = NULL;
        this->from = 0;
        this->to = 0;
        this->side = 0;
        this->castling = -1;
    }

    void Initialize(Piece *  piece, uint64_t from, uint64_t to, uint64_t side, int castling = -1) {
        this->piece = piece;
        this->from = from;
        this->to = to;
        this->side = side;
        this->castling = castling;
    }

    Piece *  piece;
    uint64_t from;
    uint64_t to;
    uint64_t side;
    int      castling;
};

#endif
