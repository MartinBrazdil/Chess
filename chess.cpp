#include <map>
#include <string>
#include <iostream>

using namespace std;

#include "bitboard.h"
#include "pieces.h"
#include "argparser.h"

/*! \mainpage Šachy (Martin Brázdil)
 *
 * \section zadani Zadání
 *
 * \subsection puvodni Původní zadání od Bc. Michala Lukáče:
 *
 * V jazyce C++ implementujte jednoduché šachy s umělou inteligencí. Vstup a výstup je pomocí algebraické šachové notace, umožnit uložení stavu, řešit konflikty, atp. Vlastní umělá inteligence nemusí být nikterak složitá, stačí MINIMAX či neco podobně jednoduchého. 
 *
 * \subsection vylepsene Zadání jsem vylepšil:
 *
 * Efektivní reprezentace šachovnice 64 bitovým integerem.
 *
 * Paralelní zpracování výpočtu pomocí vláken.
 *
 * Možnost výběru strany.
 *
 * Možnost spuštění programu s parametry, nechybí nápověda.
 *
 * \section prirucka Uživatelská příručka
 *
 * Podrobná uživatelská příručka je přístupná při spuštění programu s parametrem -h.
 *
 * \section instalace Instalační příručka
 *
 * \subsection makefile Makefile nabízí tyto možnosti kompilace:
 *
 * make all - kompilace a linkování, výsledkem spustitelný soubor "chess"
 *
 * make clean - čistá kompilace (smaže všechny .o soubory), výsledkem je spustitelný soubor "chess"
 *
 * make run - totožné s clean, s tím rozdílem, že na konci dojde ke spuštění (bez parametrů)
 *  
 * \section popis Popis speciálních funkcí a algoritmů
 *
 * Šachovnice je reprezentována 64 bitovým integerem. Přestože jsem program navrhl objektově, tak na nejnižších urovních pracuji s bitovými operacemi.
 *
 * AI je řízena (kaskádně) rekurzivním MiniMax algoritmem, který počítá materiální zisk a ztrátu jednotlivých větví. AI vidí (defaultně) 5 tahů dopředu.
 *
 * Paralelní zpracování je řízené třídou AbstractThread (z mého jiného projektu) ze které ChessAI dědí. Stačí implementovat abstraktní metody ThreadWorker a Copy.
 *
 * Všechny figurky mají společnou nadtřídu Piece, která mi umožňuje využívat polymorfismus a pracovat s figurkami v jistých ohledech stejně.
 *
 * Všechny tahy jsou generovány v konstantním čase, všechny diagonální a horizontálně-vertikální tahy jsou uloženy v mapě, která se na začátku hry vygeneruje.
 *
 * Save a load funguje na principu uložení struktury reprezentující šachovnici do souboru, je ošetřeno jak přepsání uložené pozice tak i načítání z neexistujícího souboru.
 * 
 *
 * \section zdroje Popis cizích zdrojů
 *
 * Největší inspirací mi byl web http://chessprogramming.wikispaces.com/
 * 
 * \section zaver Závěr
 *
 * Návrh tříd by šel určitě vylepšit. Výpočetní výkon je slušný, ale z důvodu použití jednoduchého MiniMax algoritmu není AI příliš konkurenceschopná. AI má největší problémy ze začátku hry, což by šlo zlepšit implementací šachových zahájení. Po menších úpravách by se toto dílo dalo považovat za solidní šachový základ.
 *
 */

int main(int argc, char** argv) {
    ArgParser Arguments(argc, argv);
    try {
        if(!Arguments.Parse()) return 0;
    } catch (const char * e) {
        cout << e << endl;
        return 0;
    }

    GlobalLookupTablesInit();

    BitBoard * Board = new BitBoard();
    
    Board->InitBoard();
    
    Board->CommandListener();

    delete Board;

    return 0;
}
