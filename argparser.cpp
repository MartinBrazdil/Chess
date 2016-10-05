#include <string>
#include <iostream>

using namespace std;

#include "argparser.h"

map<string,int> ArgParser::setArgs;

ArgParser::ArgParser ( int argc, char** argv ) {
    this->argc = argc;
    this->argv = argv;
    wrongArgsException = "Chybne zadane argumenty.";
    validArgs.insert(pair<string,string>("-d","depth"));
    validArgs.insert(pair<string,string>("-i","info"));
    validArgs.insert(pair<string,string>("-s","side"));
}

bool ArgParser::Parse ( ) {
    if(argc == 2) {
        string help = "-h";
        if(help.compare(argv[1]) != 0) throw wrongArgsException;
        PrintHelp();
        return false;
    }
    for(int i = 2; i < argc; i +=2) {
        if(!Validate(i-1, i)) throw wrongArgsException;
    }
    return true;
}

bool ArgParser::Validate ( int par, int val ) {
    if(validArgs.find(argv[par]) != validArgs.end() && atoi(argv[val]) >= 0) {
        setArgs.insert(pair<string,int>(validArgs.find(argv[par])->second,atoi(argv[val])));
        return true;
    }
    return false;
}

void ArgParser::PrintHelp ( ) {
    cout << "                                                             .---.                   " << endl;
    cout << "                                                            /     \\                  " << endl; 
    cout << "########################################################### \\.@-@./ #################" << endl; 
    cout << "##################### author: Martin Brázdil  ############# /`\\_/`\\ #################" << endl; 
    cout << "##################### <brazdma1@fit.cvut.cz> ############# //  _  \\\\ ################" << endl; 
    cout << "######################################################### | \\     )| ################" << endl; 
    cout << "                                                         /`\\_`>  <_/ \\               " << endl; 
    cout << "                                                      jgs\\__/'---'\\__/               " << endl;
    cout << endl << endl;
    cout << "SYNOPSIS" << endl;
    cout << "\tchess [-h]" << endl;
    cout << "\tchess [-s 0|1] [-d number] [-i 0|1]" << endl;
    cout << endl;
    cout << "OPTIONS" << endl;
    cout << "\t-h\tShows this help." << endl << endl;
    cout << "\t-s\tPlayer side (valid value: 0 (black)|1(white), default 1)." << endl << endl;
    cout << "\t-d\tDepth of a bruteforce search (valid value: >= 1, default 5)." << endl << endl;
    cout << "\t-i\tTurns on extra information about computation (valid value: 0|1, default 0)." << endl << endl;
    cout << "IN-GAME COMMANDS" << endl;
    cout << "\tCcncn\tMove in algebraic notation, case sensitive." << endl;
    cout << "\t\tC = big character, specifies selected piece (valid value: P (pawn), N (knight), B (bishop), R (rook), Q (queen) K (king))" << endl;
    cout << "\t\tc = small character, specifies file (valid value: a..h)" << endl;
    cout << "\t\tn = number, specifies rank (valid value: 1..8)" << endl;
    cout << "\t\texample: Pe2e4 (moves pawn from e2 to e4)" << endl << endl;
    cout << "\tO-O\tKingside castling." << endl << endl;
    cout << "\tO-O-O\tQueenside castling." << endl << endl;
    cout << "\tsave(x)\tSaves a game state to a file named x." << endl << endl;
    cout << "\tload(x)\tLoads a game state from a file named x." << endl << endl;
    cout << "\texit\tExits the chess." << endl << endl;
}

void ArgParser::Print ( ) {
    map<string,int>::iterator it;
    for ( it = setArgs.begin() ; it != setArgs.end(); it++ )
        cout << (*it).first << " => " << (*it).second << endl;
}
