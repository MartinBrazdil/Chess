#ifndef BITBOARD
#define BITBOARD

#include "pieces.h"
#include "chessAI.h"

class BitBoard {
    
private:

    Values                  MainBoard;
    bool                    HumanSideWhite;
    uint64_t *              HumanSide;
    uint64_t *              ComputerSide;

    bool                    GetFilename             ( string command, string & filename );
    bool                    SaveGame                ( string filename, Values & MainBoard );
    bool                    CheckFileExists         ( string filename );
    bool                    OverwritingSavedGame    ( string filename );
    bool                    LoadGame                ( string filename, Values & MainBoard );
    void                    AssignSides             ( );
    bool                    HumanPlayer             ( ChessAI& validator );
    bool                    ComputerPlayer          ( ChessAI& computer );
    
public:

                            BitBoard                ( );    
    void                    InitBoard               ( );
    void                    CommandListener         ( );
    static double           MovePiece               ( uint64_t & Piece, uint64_t From, uint64_t To, uint64_t & Side, Values &ChosenBoard );
    static double           MovePiece               ( int move, uint64_t & side, Values & chosenBoard );
    static uint64_t &       GetPiece                ( char Symbol, uint64_t Position, Values &ChosenBoard );
    static uint64_t &       GetPiece                ( int Side, uint64_t Pieces, Values &ChosenBoard );
    static double           GetScore                ( int Side, uint64_t Pieces, Values &ChosenBoard );
    static uint64_t         GetFile                 ( char Coordinate );
    static int              GetCoordinate           ( uint64_t Pos );
    static const char *     EvalSquare              ( uint64_t Square, Values &ChosenBoard );
    static void             ShowBoard               ( Values &ChosenBoard );
};

#endif  
