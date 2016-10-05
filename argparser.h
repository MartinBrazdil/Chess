#ifndef ARGPARSER
#define ARGPARSER

#include <map>
#include <string>

class ArgParser {

private:

    int                     argc;
    char **                 argv;
    map<string,string>      validArgs;
    const char *            wrongArgsException;

    bool                    Validate           ( int par, int val );
    void                    PrintHelp          ( );

public:
    
    static map<string,int>  setArgs;

                            ArgParser           ( int argc, char** argv );
    bool                    Parse               ( );
    void                    Print               ( );
};

#endif
