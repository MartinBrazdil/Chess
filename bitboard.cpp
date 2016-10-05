#include <map>
#include <string>
#include <chrono> 
#include <fstream>
#include <iostream>
#include <bitset>

using namespace std;
using namespace chrono;

#include "constants.h"
#include "pieces.h"
#include "chessAI.h"
#include "argparser.h"

#include "bitboard.h"

BitBoard::BitBoard() {
    MainBoard.Board      =    0x0000000000000000LLU;
    MainBoard.BPieces    =    0x0000000000000000LLU;
    MainBoard.WPieces    =    0x0000000000000000LLU;

    HumanSideWhite = true;
    if(ArgParser::setArgs.find("side") != ArgParser::setArgs.end()) {
        if(ArgParser::setArgs.find("side")->second == 0) {
            HumanSideWhite = false;
        }
    }
}

void BitBoard::InitBoard() {
    MainBoard.BQueensideCastling  =    true;
    MainBoard.BKingsideCastling   =    true;
    MainBoard.WQueensideCastling  =    true;
    MainBoard.WKingsideCastling   =    true;

    MainBoard.WPieces             =    0x000000000000FFFFLLU;
    MainBoard.BPieces             =    0xFFFF000000000000LLU;
    MainBoard.AllPieces           =    0xFFFF00000000FFFFLLU;
    MainBoard.WPawns.position     =    0x000000000000FF00LLU;
    MainBoard.WKnights.position   =    0x0000000000000042LLU;
    MainBoard.WBishops.position   =    0x0000000000000024LLU;
    MainBoard.WRooks.position     =    0x0000000000000081LLU;
    MainBoard.WQueen.position     =    0x0000000000000008LLU;
    MainBoard.WKing.position      =    0x0000000000000010LLU;
    MainBoard.BPawns.position     =    0x00FF000000000000LLU;
    MainBoard.BKnights.position   =    0x4200000000000000LLU;
    MainBoard.BBishops.position   =    0x2400000000000000LLU;
    MainBoard.BRooks.position     =    0x8100000000000000LLU;
    MainBoard.BQueen.position     =    0x0800000000000000LLU;
    MainBoard.BKing.position      =    0x1000000000000000LLU;

    MainBoard.Board = 0xFFFF00000000FFFFLLU;
}

double BitBoard::MovePiece(uint64_t & Piece, uint64_t From, uint64_t To, uint64_t & Side, Values &ChosenBoard) {
    double score;
    if(Side & ChosenBoard.BPieces) {
        if(Piece & ChosenBoard.BKing.position) {
            ChosenBoard.BQueensideCastling = false;
            ChosenBoard.BKingsideCastling = false;
        } else if(Piece & ChosenBoard.BRooks.position & A8) {
            ChosenBoard.BQueensideCastling = false;
        } else if(Piece & ChosenBoard.BRooks.position & H8) {
            ChosenBoard.BKingsideCastling = false;
        }
        if(To & ChosenBoard.BPieces) {
            return 0;
        } else if(To & ChosenBoard.WPieces) {
            score = GetScore(0,To,ChosenBoard);
            GetPiece(0,To,ChosenBoard) ^= To;
            Piece ^= From;
            Piece |= To;
            Side ^= From;
            Side |= To;
            ChosenBoard.Board ^= From;
            ChosenBoard.Board |= To;
            ChosenBoard.WPieces ^= To;
            ChosenBoard.AllPieces = ChosenBoard.WPieces | ChosenBoard.BPieces;
            return score;
        }
    } else if(Side & ChosenBoard.WPieces) {
        if(Piece & ChosenBoard.WKing.position) {
            ChosenBoard.WQueensideCastling = false;
            ChosenBoard.WKingsideCastling = false;
        } else if(Piece & ChosenBoard.WRooks.position & A1) {
            ChosenBoard.WQueensideCastling = false;
        } else if(Piece & ChosenBoard.WRooks.position & H1) {
            ChosenBoard.WKingsideCastling = false;
        }
        if(To & ChosenBoard.WPieces) {
            return 0;
        } else if(To & ChosenBoard.BPieces) {
            score = GetScore(1,To,ChosenBoard);
            GetPiece(1,To,ChosenBoard) ^= To;
            Piece ^= From;
            Piece |= To;
            Side ^= From;
            Side |= To;
            ChosenBoard.Board ^= From;
            ChosenBoard.Board |= To;
            ChosenBoard.BPieces ^= To;
            ChosenBoard.AllPieces = ChosenBoard.BPieces | ChosenBoard.WPieces;
            return score;
        }
    }
    Piece ^= From;
    Piece |= To;
    Side ^= From;
    Side |= To;
    ChosenBoard.Board ^= From;
    ChosenBoard.Board |= To;
    ChosenBoard.AllPieces = ChosenBoard.WPieces | ChosenBoard.BPieces;
    return 0;
}

double BitBoard::MovePiece(int move, uint64_t & side, Values & chosenBoard) {
    if(side == chosenBoard.WPieces) {
        //white queenside castling
        if(move == 0) {
            MovePiece(chosenBoard.WKing.position, E1, C1, side, chosenBoard);
            MovePiece(chosenBoard.WRooks.position, A1, D1, side, chosenBoard);
        }//white kingside castling
        else if(move == 1) {
            MovePiece(chosenBoard.WKing.position, E1, G1, side, chosenBoard);
            MovePiece(chosenBoard.WRooks.position, H1, F1, side, chosenBoard);
        }
    } else if(side == chosenBoard.BPieces) {
        //black queenside castling
        if(move == 0) {
            MovePiece(chosenBoard.BKing.position, E8, C8, side, chosenBoard);
            MovePiece(chosenBoard.BRooks.position, A8, D8, side, chosenBoard);
        }//black kingside castling
        else if(move == 1) {
            MovePiece(chosenBoard.BKing.position, E8, G8, side, chosenBoard);
            MovePiece(chosenBoard.BRooks.position, H8, F8, side, chosenBoard);
        }
    }
    return 5;
}

uint64_t & BitBoard::GetPiece(char Symbol, uint64_t Position, Values & ChosenBoard) {
    if(Symbol == 'N') {
        if(Position & ChosenBoard.WKnights.position) {
            return ChosenBoard.WKnights.position;
        } else {
            return ChosenBoard.BKnights.position;
        }
    }
    else if(Symbol == 'B') {
        if(Position & ChosenBoard.WBishops.position) {
            return ChosenBoard.WBishops.position;
        } else {
            return ChosenBoard.BBishops.position;
        }
    }
    else if(Symbol == 'R') {
        if(Position & ChosenBoard.WRooks.position) {
            return ChosenBoard.WRooks.position;
        } else {
            return ChosenBoard.BRooks.position;
        }
    }
    else if(Symbol == 'Q') {
        if(Position & ChosenBoard.WQueen.position) {
            return ChosenBoard.WQueen.position;
        } else {
            return ChosenBoard.BQueen.position;
        }
    }
    else if(Symbol == 'K') {
        if(Position & ChosenBoard.WKing.position) {
            return ChosenBoard.WKing.position;
        } else {
            return ChosenBoard.BKing.position;
        }
    }
    else if(Symbol == 'P') {
        if(Position & ChosenBoard.WPawns.position) {
            return ChosenBoard.WPawns.position;
        } else {
            return ChosenBoard.BPawns.position;
        }
    }
    return ChosenBoard.Board;
}

double BitBoard::GetScore(int Side, uint64_t Pieces, Values &ChosenBoard) {
    if(Side) {
        if(Pieces & ChosenBoard.BPawns.position  ) return 1;
        if(Pieces & ChosenBoard.BRooks.position  ) return 5.1;
        if(Pieces & ChosenBoard.BKnights.position) return 3.2;
        if(Pieces & ChosenBoard.BBishops.position) return 3.33;
        if(Pieces & ChosenBoard.BQueen.position  ) return 8.8;
        return 9;
    } else {
        if(Pieces & ChosenBoard.WPawns.position  ) return 1;
        if(Pieces & ChosenBoard.WRooks.position  ) return 5.1;
        if(Pieces & ChosenBoard.WKnights.position) return 3.2;
        if(Pieces & ChosenBoard.WBishops.position) return 3.33;
        if(Pieces & ChosenBoard.WQueen.position  ) return 8.8;
        return 9;
    }
}

uint64_t & BitBoard::GetPiece(int Side, uint64_t Pieces, Values &ChosenBoard) {
    if(Side) {
        if(Pieces & ChosenBoard.BPawns.position  ) return ChosenBoard.BPawns.position;
        if(Pieces & ChosenBoard.BRooks.position  ) return ChosenBoard.BRooks.position;
        if(Pieces & ChosenBoard.BKnights.position) return ChosenBoard.BKnights.position;
        if(Pieces & ChosenBoard.BBishops.position) return ChosenBoard.BBishops.position;
        if(Pieces & ChosenBoard.BQueen.position  ) return ChosenBoard.BQueen.position;
                                                   return ChosenBoard.BKing.position;
    } else {
        if(Pieces & ChosenBoard.WPawns.position  ) return ChosenBoard.WPawns.position;
        if(Pieces & ChosenBoard.WRooks.position  ) return ChosenBoard.WRooks.position;
        if(Pieces & ChosenBoard.WKnights.position) return ChosenBoard.WKnights.position;
        if(Pieces & ChosenBoard.WBishops.position) return ChosenBoard.WBishops.position;
        if(Pieces & ChosenBoard.WQueen.position  ) return ChosenBoard.WQueen.position;
                                                   return ChosenBoard.WKing.position;
    }

}

uint64_t BitBoard::GetFile(char Coordinate) {
    int index = (int)Coordinate;
    if(97 <= index && index <= 104) {
        return (uint64_t)(index-96);
    } return -1;
}

int BitBoard::GetCoordinate(uint64_t pos) {
    int posNum = 0;
    for(int i = 0; (pos>>i)!=1; i++) {
        posNum++;
    }
    return posNum;
}

bool BitBoard::GetFilename(string command, string & filename) {
    unsigned begin = command.find_first_of('(');
    unsigned end = command.find_first_of(')');
    if(begin == string::npos || end == string::npos) {
        return false;
    }
    filename = command.substr(begin+1,end-(begin+1));
    return true;
}

bool BitBoard::CheckFileExists(string filename) {
    ifstream file(filename, ifstream::in);
    if(file.fail()) return false;
    file.close();
    return true;
}

bool BitBoard::OverwritingSavedGame(string filename) {
    if(CheckFileExists(filename)) {
        char answer;
        do {
            cout << "> Do you want overwrite the saved game \"" << filename << "\"? (y/n)" << endl;
            cout << "> ";
            cin >> answer;
            if(answer == 'y') {
                return true;
            }    
            if(answer == 'n') {
                return false;
            }
        } while(1);
    }
    return true;
}

bool BitBoard::SaveGame(string filename, Values & MainBoard) {
    ofstream savefile(filename, ofstream::out);

    if (savefile.is_open()) {
        savefile << MainBoard.BQueensideCastling << "\n";
        savefile << MainBoard.BKingsideCastling << "\n";
        savefile << MainBoard.WQueensideCastling << "\n";
        savefile << MainBoard.WKingsideCastling << "\n";

        savefile << MainBoard.AllPieces << "\n";
        savefile << MainBoard.WPieces << "\n";
        savefile << MainBoard.BPieces << "\n";

        savefile << MainBoard.WPawns.position << "\n";
        savefile << MainBoard.WKnights.position << "\n";
        savefile << MainBoard.WBishops.position << "\n";
        savefile << MainBoard.WRooks.position << "\n";
        savefile << MainBoard.WQueen.position << "\n";
        savefile << MainBoard.WKing.position << "\n";

        savefile << MainBoard.BPawns.position << "\n";
        savefile << MainBoard.BKnights.position << "\n";
        savefile << MainBoard.BBishops.position << "\n";
        savefile << MainBoard.BRooks.position << "\n";
        savefile << MainBoard.BQueen.position  << "\n";
        savefile << MainBoard.BKing.position << "\n";

        savefile << MainBoard.Board << "\n";

        savefile.close();

        return true;
    }
    return false;
}

bool BitBoard::LoadGame(string filename, Values & MainBoard) {
    ifstream savefile(filename, ifstream::in);
    if (savefile.is_open()) {
        string line;
        if(savefile.good()) {
            try {
                //getline (savefile,line);
                savefile >> MainBoard.BQueensideCastling;
                savefile >> MainBoard.BKingsideCastling;
                savefile >> MainBoard.WQueensideCastling;
                savefile >> MainBoard.WKingsideCastling;

                savefile >> MainBoard.AllPieces;
                savefile >> MainBoard.WPieces;
                savefile >> MainBoard.BPieces;

                savefile >> MainBoard.WPawns.position;
                savefile >> MainBoard.WKnights.position;
                savefile >> MainBoard.WBishops.position;
                savefile >> MainBoard.WRooks.position;
                savefile >> MainBoard.WQueen.position;
                savefile >> MainBoard.WKing.position;

                savefile >> MainBoard.BPawns.position;
                savefile >> MainBoard.BKnights.position;
                savefile >> MainBoard.BBishops.position;
                savefile >> MainBoard.BRooks.position;
                savefile >> MainBoard.BQueen.position;
                savefile >> MainBoard.BKing.position;

                savefile >> MainBoard.Board;

                savefile.close();
                return true;
            } catch(const char * e) {
                return false;
            }
        }
    }
    return false;
}

void BitBoard::AssignSides() {
    switch(HumanSideWhite) {
        case false : {
            HumanSide = &MainBoard.BPieces;
            ComputerSide = &MainBoard.WPieces;
            break;
        } 
        case true : {
            HumanSide = &MainBoard.WPieces;
            ComputerSide = &MainBoard.BPieces;
            break;
        }
    }
}

bool BitBoard::HumanPlayer(ChessAI& validator) {
    uint64_t FromFile, FromRank, ToFile, ToRank;
    int To = 0, From = 0;
    string command;
    Move dummy;
    dummy.piece = NULL;

    multimap<double,Move>::iterator validatorIt;
    validator.AIChooseNextMove(HumanSideWhite, 0, dummy, MainBoard, 0, 0);

    if(validator.results.size() == 0) {
        cout << "> Checkmate!" << endl;
        return false;
    }

    do {
        cout << "> ";
        cin >> command;

        if(command == "exit") return false;
        if(command.substr(0,4) == "save") {
            string filename;
            if(!GetFilename(command, filename)) {
                cout << "> Invalid save syntax. Retry." << endl;
                continue;
            }
            if(OverwritingSavedGame(filename)) {
                if(SaveGame(filename, MainBoard)) {
                    cout << "> Game has been saved as \"" << filename << "\"." << endl;
                    continue;
                }
                cout << "> Game save crashed. Retry." << endl;
            } else {
                cout << "> Game has not been saved." << endl;
                continue;
            }
        }
        if(command.substr(0,4) == "load") {
            string filename;
            if(!GetFilename(command, filename)) {
                cout << "> Invalid save syntax. Retry." << endl;
                continue;
            }
            if(LoadGame(filename, MainBoard)) {
                cout << "> Game has been successfully loaded from \"" << filename << "\"." << endl;
                CommandListener();
            } else {
                cout << "> Game load crashed. Retry." << endl;
                continue;
            }
        }
        if(command == "O-O-O") {
            validatorIt = validator.results.begin();
            while(validatorIt != validator.results.end()) {
                if(validatorIt->second.castling == 0)
                {
                    MovePiece(0, (*HumanSide), MainBoard);
                    return true;
                }
                validatorIt++;
            }
            cout << "> Invalid move \"queenside castling\". Retry." << endl;
            continue;
        }
        if(command == "O-O") {
            validatorIt = validator.results.begin();
            while(validatorIt != validator.results.end()) {
                if(validatorIt->second.castling == 1)
                {
                    MovePiece(1, (*HumanSide), MainBoard);
                    return true;
                }
                validatorIt++;
            }
            cout << "> Invalid move \"kingside castling\". Retry." << endl;
            continue;
        }
        if(command.substr(0,1).find_first_of("PNBRQK") == string::npos) {
            cout << "> Invalid piece \""<<command[0]<<"\", position "<<1<<". Retry." << endl;
            continue;
        }
        if((int)GetFile(command[1]) != -1) {
            FromFile = GetFile(command[1]);
        } else {
            cout << "> Invalid file \""<<command[1]<<"\", position "<<2<<". Retry." << endl;
            continue;
        }
        if(atoi(&command[2]) > 0 && atoi(&command[2]) < 9) {
            FromRank = atoi(&command[2]);
        } else {
            cout << "> Invalid rank \""<<command[2]<<"\", position "<<3<<". Retry." << endl;
            continue;
        }
        if((int)GetFile(command[3]) != -1) {
            ToFile = GetFile(command[3]);
        } else {
            cout << "> Invalid file \""<<command[3]<<"\", position "<<4<<". Retry." << endl;
            continue;
        }
        if(atoi(&command[4]) > 0 && atoi(&command[4]) < 9) {
            ToRank = atoi(&command[4]);
        } else {
            cout << "> Invalid rank \""<<command[4]<<"\", position "<<5<<". Retry." << endl;
            continue;
        }
        From = (FromFile-1)+(FromRank-1)*8;
        To = (ToFile-1)+(ToRank-1)*8;

        validatorIt = validator.results.begin();
        while(validatorIt != validator.results.end()) {
            if(validatorIt->second.piece->position == GetPiece(command[0], Squares[From], MainBoard) &&
               validatorIt->second.from == Squares[From] &&
               validatorIt->second.to == Squares[To] &&
               validatorIt->second.side == (*HumanSide) &&
               validatorIt->second.castling == -1 )
            {
                MovePiece(GetPiece(command[0], Squares[From], MainBoard), Squares[From], Squares[To], (*HumanSide), MainBoard);
                return true;
            }
            validatorIt++;
        }
        cout << "> Invalid move. Retry." << endl;
    } while(1);
    return false;
}

bool BitBoard::ComputerPlayer(ChessAI& computer) {
    Move dummy;
    dummy.piece = NULL;
    high_resolution_clock::time_point t1, t2;

    if(ArgParser::setArgs["info"] == 1) {
        t1 = high_resolution_clock::now();
    }

    computer.AIChooseNextMove(!HumanSideWhite, 0, dummy, MainBoard, 0, 0);

    if(ArgParser::setArgs["info"] == 1) {
        t2 = high_resolution_clock::now();
        cout << "Elapsed time: " << duration_cast<milliseconds>(t2-t1).count() << " ms.\n";
    }

    if(computer.results.size() == 0) {
        cout << "> Checkmate!" << endl;
        return false;
    }

    multimap<double,Move>::iterator it = computer.results.begin();
    pair<double,Move> bestMove = *it;
    while(it != computer.results.end()) {
        if((*it).first > bestMove.first) {
            bestMove = *it;
            cout << it->first << endl;
        }
        it++;
    }

    if(bestMove.second.castling >= 0) {
        MovePiece(bestMove.second.castling, (*ComputerSide), MainBoard);
    } else {
        MovePiece(bestMove.second.piece->GetPiece(MainBoard), bestMove.second.from, bestMove.second.to, *ComputerSide, MainBoard);
    }
    return true;
}

void BitBoard::CommandListener() {
    AssignSides();
    
    while(1) {
        ChessAI computer{};
        ChessAI validator{1};

        if(HumanSideWhite) {
            ShowBoard(MainBoard);
            if(!HumanPlayer(validator)) return;
            ShowBoard(MainBoard);
            if(!ComputerPlayer(computer)) return; 
        } else {
            ShowBoard(MainBoard);
            if(!ComputerPlayer(computer)) return;
            ShowBoard(MainBoard);
            if(!HumanPlayer(validator)) return;
        }
    }
}

const char * BitBoard::EvalSquare(uint64_t square, Values &ChosenBoard) {
    if(square & ChosenBoard.Board & ChosenBoard.WPieces) {
        if(square & ChosenBoard.Board & ChosenBoard.WPawns.position  ) return "WP";
        if(square & ChosenBoard.Board & ChosenBoard.WKnights.position) return "WN";
        if(square & ChosenBoard.Board & ChosenBoard.WBishops.position) return "WB";
        if(square & ChosenBoard.Board & ChosenBoard.WRooks.position  ) return "WR";
        if(square & ChosenBoard.Board & ChosenBoard.WKing.position   ) return "WK";
        if(square & ChosenBoard.Board & ChosenBoard.WQueen.position  ) return "WQ";
    }
    if(square & ChosenBoard.Board & ChosenBoard.BPieces) {
        if(square & ChosenBoard.Board & ChosenBoard.BPawns.position  ) return "BP";
        if(square & ChosenBoard.Board & ChosenBoard.BKnights.position) return "BN";
        if(square & ChosenBoard.Board & ChosenBoard.BBishops.position) return "BB";
        if(square & ChosenBoard.Board & ChosenBoard.BRooks.position  ) return "BR";
        if(square & ChosenBoard.Board & ChosenBoard.BKing.position   ) return "BK";
        if(square & ChosenBoard.Board & ChosenBoard.BQueen.position  ) return "BQ";
    }
    return "  ";
}

void BitBoard::ShowBoard(Values &ChosenBoard) {
    cout << "       A     B    C    D    E    F    G    H" << endl;
    cout << "  |X||X||X||X||X||X||X||X||X||X||X||X||X||X|X|X|X|" << endl;
    cout << "  |X||X||X||X||X||X||X||X||X||X||X||X||X||X|X|X|X|" << endl;
    cout << "  |X|/----|----|----|----|----|----|----|----\\|X||" << endl;
    cout << "8 |X|| "<<EvalSquare(A8, ChosenBoard)<<" | "<<EvalSquare(B8, ChosenBoard)<<" | "<<EvalSquare(C8, ChosenBoard)<<" | "<<EvalSquare(D8, ChosenBoard)<<" | "<<EvalSquare(E8, ChosenBoard)<<" | "<<EvalSquare(F8, ChosenBoard)<<" | "<<EvalSquare(G8, ChosenBoard)<<" | "<<EvalSquare(H8, ChosenBoard)<<" ||X|| 8" << endl;
    cout << "  |X||____|____|____|____|____|____|____|____||X||" << endl;
    cout << "  |X||    |    |    |    |    |    |    |    ||X||" << endl;
    cout << "7 |X|| "<<EvalSquare(A7, ChosenBoard)<<" | "<<EvalSquare(B7, ChosenBoard)<<" | "<<EvalSquare(C7, ChosenBoard)<<" | "<<EvalSquare(D7, ChosenBoard)<<" | "<<EvalSquare(E7, ChosenBoard)<<" | "<<EvalSquare(F7, ChosenBoard)<<" | "<<EvalSquare(G7, ChosenBoard)<<" | "<<EvalSquare(H7, ChosenBoard)<<" ||X|| 7" << endl;
    cout << "  |X||____|____|____|____|____|____|____|____||X||" << endl;
    cout << "  |X||    |    |    |    |    |    |    |    ||X||" << endl;
    cout << "6 |X|| "<<EvalSquare(A6, ChosenBoard)<<" | "<<EvalSquare(B6, ChosenBoard)<<" | "<<EvalSquare(C6, ChosenBoard)<<" | "<<EvalSquare(D6, ChosenBoard)<<" | "<<EvalSquare(E6, ChosenBoard)<<" | "<<EvalSquare(F6, ChosenBoard)<<" | "<<EvalSquare(G6, ChosenBoard)<<" | "<<EvalSquare(H6, ChosenBoard)<<" ||X|| 6" << endl;
    cout << "  |X||____|____|____|____|____|____|____|____||X||" << endl;
    cout << "  |X||    |    |    |    |    |    |    |    ||X||" << endl;
    cout << "5 |X|| "<<EvalSquare(A5, ChosenBoard)<<" | "<<EvalSquare(B5, ChosenBoard)<<" | "<<EvalSquare(C5, ChosenBoard)<<" | "<<EvalSquare(D5, ChosenBoard)<<" | "<<EvalSquare(E5, ChosenBoard)<<" | "<<EvalSquare(F5, ChosenBoard)<<" | "<<EvalSquare(G5, ChosenBoard)<<" | "<<EvalSquare(H5, ChosenBoard)<<" ||X|| 5" << endl;
    cout << "  |X||____|____|____|____|____|____|____|____||X||" << endl;
    cout << "  |X||    |    |    |    |    |    |    |    ||X||" << endl;
    cout << "4 |X|| "<<EvalSquare(A4, ChosenBoard)<<" | "<<EvalSquare(B4, ChosenBoard)<<" | "<<EvalSquare(C4, ChosenBoard)<<" | "<<EvalSquare(D4, ChosenBoard)<<" | "<<EvalSquare(E4, ChosenBoard)<<" | "<<EvalSquare(F4, ChosenBoard)<<" | "<<EvalSquare(G4, ChosenBoard)<<" | "<<EvalSquare(H4, ChosenBoard)<<" ||X|| 4" << endl;
    cout << "  |X||____|____|____|____|____|____|____|____||X||" << endl;
    cout << "  |X||    |    |    |    |    |    |    |    ||X||" << endl;
    cout << "3 |X|| "<<EvalSquare(A3, ChosenBoard)<<" | "<<EvalSquare(B3, ChosenBoard)<<" | "<<EvalSquare(C3, ChosenBoard)<<" | "<<EvalSquare(D3, ChosenBoard)<<" | "<<EvalSquare(E3, ChosenBoard)<<" | "<<EvalSquare(F3, ChosenBoard)<<" | "<<EvalSquare(G3, ChosenBoard)<<" | "<<EvalSquare(H3, ChosenBoard)<<" ||X|| 3" << endl;
    cout << "  |X||____|____|____|____|____|____|____|____||X||" << endl;
    cout << "  |X||    |    |    |    |    |    |    |    ||X||" << endl;
    cout << "2 |X|| "<<EvalSquare(A2, ChosenBoard)<<" | "<<EvalSquare(B2, ChosenBoard)<<" | "<<EvalSquare(C2, ChosenBoard)<<" | "<<EvalSquare(D2, ChosenBoard)<<" | "<<EvalSquare(E2, ChosenBoard)<<" | "<<EvalSquare(F2, ChosenBoard)<<" | "<<EvalSquare(G2, ChosenBoard)<<" | "<<EvalSquare(H2, ChosenBoard)<<" ||X|| 2" << endl;
    cout << "  |X||____|____|____|____|____|____|____|____||X||" << endl;
    cout << "  |X||    |    |    |    |    |    |    |    ||X||" << endl;
    cout << "1 |X|| "<<EvalSquare(A1, ChosenBoard)<<" | "<<EvalSquare(B1, ChosenBoard)<<" | "<<EvalSquare(C1, ChosenBoard)<<" | "<<EvalSquare(D1, ChosenBoard)<<" | "<<EvalSquare(E1, ChosenBoard)<<" | "<<EvalSquare(F1, ChosenBoard)<<" | "<<EvalSquare(G1, ChosenBoard)<<" | "<<EvalSquare(H1, ChosenBoard)<<" ||X|| 1" << endl;
    cout << "  |X|\\____|____|____|____|____|____|____|____/|X||" << endl;
    cout << "  |X||X||X||X||X||X||X||X||X||X||X||X||X||X|X|X|X|" << endl;
    cout << "  |X||X||X||X||X||X||X||X||X||X||X||X||X||X|X|X|X|" << endl;
    cout << "       A     B    C    D    E    F    G    H" << endl;
}
