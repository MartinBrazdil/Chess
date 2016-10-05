#include <iostream>

using namespace std;

#include "pieces.h"
#include "constants.h"
#include "lookuptables.h"

map<uint64_t,uint64_t> ** RankAttacks     = new map<uint64_t,uint64_t>*[64];
map<uint64_t,uint64_t> ** FileAttacks     = new map<uint64_t,uint64_t>*[64];
map<uint64_t,uint64_t> ** DiagAttacksA1H8 = new map<uint64_t,uint64_t>*[64];
map<uint64_t,uint64_t> ** DiagAttacksH1A8 = new map<uint64_t,uint64_t>*[64];

void GlobalLookupTablesInit ( ) {
    for(int i = 0; i < 64; i++) {
        RankAttacks[i]     = new map<uint64_t,uint64_t>;
        FileAttacks[i]     = new map<uint64_t,uint64_t>;
        DiagAttacksA1H8[i] = new map<uint64_t,uint64_t>;
        DiagAttacksH1A8[i] = new map<uint64_t,uint64_t>;
    }

    InitLookupTables(RankAttacks, FileAttacks, DiagAttacksA1H8, DiagAttacksH1A8);
}

Piece::Piece() { }

int Piece::Split ( uint64_t ToSplit, uint64_t * Pieces ) {
    int counter = 0;
    for(int i = 0; i < 64; i++) {
        if((Squares[i]) & ToSplit) {
                Pieces[counter++] = Squares[i];
        }
    }
    return counter;
}

int Piece::GetCoordinate ( const uint64_t Position ) const {
    int numericPosition = 0;
    for(int i = 0; (Position>>i)!=1; i++) {
            numericPosition++;
    }
    return numericPosition;
}

uint64_t & PawnsW::GetPiece   ( Values &ChosenBoard ) { return ChosenBoard.WPawns.position; }
uint64_t & PawnsB::GetPiece   ( Values &ChosenBoard ) { return ChosenBoard.BPawns.position; }
uint64_t & KnightsW::GetPiece ( Values &ChosenBoard ) { return ChosenBoard.WKnights.position; }
uint64_t & KnightsB::GetPiece ( Values &ChosenBoard ) { return ChosenBoard.BKnights.position; }
uint64_t & BishopsW::GetPiece ( Values &ChosenBoard ) { return ChosenBoard.WBishops.position; }
uint64_t & BishopsB::GetPiece ( Values &ChosenBoard ) { return ChosenBoard.BBishops.position; }
uint64_t & RooksW::GetPiece   ( Values &ChosenBoard ) { return ChosenBoard.WRooks.position; }
uint64_t & RooksB::GetPiece   ( Values &ChosenBoard ) { return ChosenBoard.BRooks.position; }
uint64_t & QueenW::GetPiece   ( Values &ChosenBoard ) { return ChosenBoard.WQueen.position; }
uint64_t & QueenB::GetPiece   ( Values &ChosenBoard ) { return ChosenBoard.BQueen.position; }
uint64_t & KingW::GetPiece    ( Values &ChosenBoard ) { return ChosenBoard.WKing.position; }
uint64_t & KingB::GetPiece    ( Values &ChosenBoard ) { return ChosenBoard.BKing.position; }

uint64_t BishopsW::AllMoves ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) {
    uint64_t allMoves = 0;
    int posNum = GetCoordinate(pieces[n]);
    allMoves |= DiagAttacksA1H8[posNum]->find(Board & MaskDiagA1H8[DiagA1H8[posNum]])->second;
    allMoves |= DiagAttacksH1A8[posNum]->find(Board & MaskDiagH1A8[DiagH1A8[posNum]])->second;
    allMoves ^= (allMoves & WPieces);
    validMoves = allMoves;
    return validMoves;
}

uint64_t BishopsB::AllMoves ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) {
    uint64_t allMoves = 0;
    int posNum = GetCoordinate(pieces[n]);
    allMoves |= DiagAttacksA1H8[posNum]->find(Board & MaskDiagA1H8[DiagA1H8[posNum]])->second;
    allMoves |= DiagAttacksH1A8[posNum]->find(Board & MaskDiagH1A8[DiagH1A8[posNum]])->second;
    allMoves ^= (allMoves & BPieces);
    validMoves = allMoves;
    return validMoves;
}

uint64_t RooksW::AllMoves ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) {
    uint64_t allMoves = 0;
    int posNum = GetCoordinate(pieces[n]);
    allMoves |= RankAttacks[posNum]->find(Board & MaskRank[Rank[posNum]])->second;
    allMoves |= FileAttacks[posNum]->find(Board & MaskFile[File[posNum]])->second;
    allMoves ^= (allMoves & WPieces);
    validMoves = allMoves;
    return validMoves;
}

uint64_t RooksB::AllMoves ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) {
    uint64_t allMoves = 0;
    int posNum = GetCoordinate(pieces[n]);
    allMoves |= RankAttacks[posNum]->find(Board & MaskRank[Rank[posNum]])->second;
    allMoves |= FileAttacks[posNum]->find(Board & MaskFile[File[posNum]])->second;
    allMoves ^= (allMoves & BPieces);
    validMoves = allMoves;
    return validMoves;
}

uint64_t QueenW::AllMoves ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) {
    uint64_t allMoves = 0;
    int posNum = GetCoordinate(pieces[n]);
    allMoves |= RankAttacks[posNum]->find(Board & MaskRank[Rank[posNum]])->second;
    allMoves |= FileAttacks[posNum]->find(Board & MaskFile[File[posNum]])->second;
    allMoves |= DiagAttacksA1H8[posNum]->find(Board & MaskDiagA1H8[DiagA1H8[posNum]])->second;
    allMoves |= DiagAttacksH1A8[posNum]->find(Board & MaskDiagH1A8[DiagH1A8[posNum]])->second;
    allMoves ^= (allMoves & WPieces);
    validMoves = allMoves;
    return validMoves;
}

uint64_t QueenB::AllMoves ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) {
    uint64_t allMoves = 0;
    int posNum = GetCoordinate(pieces[n]);
    allMoves |= RankAttacks[posNum]->find(Board & MaskRank[Rank[posNum]])->second;
    allMoves |= FileAttacks[posNum]->find(Board & MaskFile[File[posNum]])->second;
    allMoves |= DiagAttacksA1H8[posNum]->find(Board & MaskDiagA1H8[DiagA1H8[posNum]])->second;
    allMoves |= DiagAttacksH1A8[posNum]->find(Board & MaskDiagH1A8[DiagH1A8[posNum]])->second;
    allMoves ^= (allMoves & BPieces);
    validMoves = allMoves;
    return validMoves;
}

uint64_t KnightsW::AllMoves ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) {
    uint64_t ValidSquare1, ValidSquare2, ValidSquare3, ValidSquare4;
    uint64_t ValidSquare5, ValidSquare6, ValidSquare7, ValidSquare8;
    uint64_t ClipFileAB, ClipFileHG;
    uint64_t KnightValidMoves;

    ClipFileAB = ClearFile[A] & ClearFile[B];
    ClipFileHG = ClearFile[H] & ClearFile[G];

    ValidSquare1 = (pieces[n] & ClipFileAB) << 6;
    ValidSquare2 = (pieces[n] & ClearFile[A]) << 15;
    ValidSquare3 = (pieces[n] & ClearFile[H]) << 17;
    ValidSquare4 = (pieces[n] & ClipFileHG) << 10;

    ValidSquare5 = (pieces[n] & ClipFileHG) >> 6;
    ValidSquare6 = (pieces[n] & ClearFile[H]) >> 15;
    ValidSquare7 = (pieces[n] & ClearFile[A]) >> 17;
    ValidSquare8 = (pieces[n] & ClipFileAB) >> 10;

    KnightValidMoves = ValidSquare1 | ValidSquare2 | ValidSquare3 | ValidSquare4 |
                       ValidSquare5 | ValidSquare6 | ValidSquare7 | ValidSquare8;

    validMoves = KnightValidMoves & ~WPieces;
    return validMoves;
}

uint64_t KnightsB::AllMoves ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) {
    uint64_t ValidSquare1, ValidSquare2, ValidSquare3, ValidSquare4;
    uint64_t ValidSquare5, ValidSquare6, ValidSquare7, ValidSquare8;
    uint64_t ClipFileAB, ClipFileHG;
    uint64_t KnightValidMoves;

    ClipFileAB = ClearFile[A] & ClearFile[B];
    ClipFileHG = ClearFile[H] & ClearFile[G];

    ValidSquare1 = (pieces[n] & ClipFileAB) << 6;
    ValidSquare2 = (pieces[n] & ClearFile[A]) << 15;
    ValidSquare3 = (pieces[n] & ClearFile[H]) << 17;
    ValidSquare4 = (pieces[n] & ClipFileHG) << 10;

    ValidSquare5 = (pieces[n] & ClipFileHG) >> 6;
    ValidSquare6 = (pieces[n] & ClearFile[H]) >> 15;
    ValidSquare7 = (pieces[n] & ClearFile[A]) >> 17;
    ValidSquare8 = (pieces[n] & ClipFileAB) >> 10;

    KnightValidMoves = ValidSquare1 | ValidSquare2 | ValidSquare3 | ValidSquare4 |
                       ValidSquare5 | ValidSquare6 | ValidSquare7 | ValidSquare8;

    validMoves = KnightValidMoves & ~BPieces;
    return validMoves;
}

uint64_t KingW::AllMoves ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) {
    uint64_t ValidSquare1, ValidSquare2, ValidSquare3, ValidSquare4;
    uint64_t ValidSquare5, ValidSquare6, ValidSquare7, ValidSquare8;
    uint64_t ClipFileH, ClipFileA;
    uint64_t KingValidMoves;

    ClipFileH = pieces[n] & ClearFile[H];
    ClipFileA = pieces[n] & ClearFile[A];

    ValidSquare1 = ClipFileH << 7;
    ValidSquare2 = pieces[n] << 8;
    ValidSquare3 = ClipFileH << 9;
    ValidSquare4 = ClipFileH << 1;

    ValidSquare5 = ClipFileA >> 7;
    ValidSquare6 = pieces[n] >> 8;
    ValidSquare7 = ClipFileA >> 9;
    ValidSquare8 = ClipFileA >> 1;

    KingValidMoves = ValidSquare1 | ValidSquare2 | ValidSquare3 | ValidSquare4 |
                     ValidSquare5 | ValidSquare6 | ValidSquare7 | ValidSquare8;

    validMoves = KingValidMoves & ~WPieces;
    return validMoves;
}

uint64_t KingB::AllMoves ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) {
    uint64_t ValidSquare1, ValidSquare2, ValidSquare3, ValidSquare4;
    uint64_t ValidSquare5, ValidSquare6, ValidSquare7, ValidSquare8;
    uint64_t ClipFileH, ClipFileA;
    uint64_t KingValidMoves;

    ClipFileH = pieces[n] & ClearFile[H];
    ClipFileA = pieces[n] & ClearFile[A];

    ValidSquare1 = ClipFileH << 7;
    ValidSquare2 = pieces[n] << 8;
    ValidSquare3 = ClipFileH << 9;
    ValidSquare4 = ClipFileH << 1;

    ValidSquare5 = ClipFileA >> 7;
    ValidSquare6 = pieces[n] >> 8;
    ValidSquare7 = ClipFileA >> 9;
    ValidSquare8 = ClipFileA >> 1;

    KingValidMoves = ValidSquare1 | ValidSquare2 | ValidSquare3 | ValidSquare4 |
                     ValidSquare5 | ValidSquare6 | ValidSquare7 | ValidSquare8;

    validMoves = KingValidMoves & ~BPieces;
    return validMoves;
}

uint64_t PawnsB::AllMoves ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) {
    uint64_t moves, attacks;

    moves   = BPawnMoves(AllPieces, n);
    attacks = BPawnAttacks(n);

    return moves | (attacks & WPieces);
}

uint64_t PawnsB::BPawnMoves ( uint64_t AllPieces, int n ) {
    uint64_t OneStep, TwoSteps, ValidMoves;

    OneStep    = (pieces[n] >> 8) & ~AllPieces;
    TwoSteps   = ((OneStep & MaskRank[5]) >> 8) & ~AllPieces;
    ValidMoves = OneStep | TwoSteps;

    return ValidMoves;
}

uint64_t PawnsB::BPawnAttacks ( int n ) {
    uint64_t LeftAttack, RightAttack, AllAttacks, ValidAttacks;

    LeftAttack   = (pieces[n] & ClearFile[H]) >> 7;
    RightAttack  = (pieces[n] & ClearFile[A]) >> 9;
    AllAttacks   = LeftAttack | RightAttack;
    ValidAttacks = AllAttacks;

    return ValidAttacks;
}

uint64_t PawnsW::AllMoves ( uint64_t AllPieces, uint64_t BPieces, uint64_t WPieces, uint64_t Board, int n ) {
    uint64_t moves, attacks;

    moves   = WPawnMoves(AllPieces, n);
    attacks = WPawnAttacks(n);

    return moves | (attacks & BPieces);
}

uint64_t PawnsW::WPawnMoves ( uint64_t AllPieces, int n ) {
    uint64_t OneStep, TwoSteps, ValidMoves;

    OneStep    = (pieces[n] << 8) & ~AllPieces;
    TwoSteps   = ((OneStep & MaskRank[2]) << 8) & ~AllPieces;
    ValidMoves = OneStep | TwoSteps;

    return ValidMoves;
}

uint64_t PawnsW::WPawnAttacks ( int n ) {
    uint64_t LeftAttack, RightAttack, AllAttacks, ValidAttacks;

    LeftAttack   = (pieces[n] & ClearFile[A]) << 7;
    RightAttack  = (pieces[n] & ClearFile[H]) << 9;
    AllAttacks   = LeftAttack | RightAttack;
    ValidAttacks = AllAttacks;

    return ValidAttacks;
}
