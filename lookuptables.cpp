#include <map>
#include <string>
#include <iostream>
#include <cmath>

using namespace std;

#include "constants.h"

void ConvBasePrint(uint64_t v, long base = 2)
{
    string digits = "0123456789abcdef";
    string result;
    if((base < 2) || (base > 16)) {
        result = "Error: base out of range.";
    }
    else {
        do {
            result = digits[v % base] + result;
            v /= base;
        }
        while(v);
    }
    cout << result;
}

// map<uint64_t,uint64_t> ** RankAttacks     = new map<uint64_t,uint64_t>*[64];
void HashRankAttacks(map<uint64_t,uint64_t> ** RankAttacks) {
    for(int i = 0; i < 8; i++) {
        uint64_t pos = 1<<i;
        for(int x = 0; x < 256; x++) {
            uint64_t right, left, value = 0;
            for(right = pos; ;right>>=1) {
                if(((right & x) == 0)) value |= right;
                if(((right & x) != 0) || (right == 0)) {
                    break;
                }
            }
            for(left = pos; ;left<<=1) {
                if(((left & x) == 0)) value |= left;
                if(((left & x) != 0) || (left > 127)) {
                    if(left>256) left = 0;
                    break;
                }
            }
            if((value&pos)==0) value |= pos;
            value |= (left|right);
            RankAttacks[i]->insert(pair<uint64_t,uint64_t>(pos^x,value));
            for(int rank = 0; rank < 8; rank++) {
                RankAttacks[i+rank*8]->insert(pair<uint64_t,uint64_t>((pos^x)<<(rank*8),(value)<<(rank*8)));
            }
        }
    }
}

int TransformCoordinate(int x) {
    int count8Diff=0, count1Diff=0, result = 7;
    while(x-8 >= 0) {
        count8Diff++;
        x -= 8;
    }
    while(x-1 >= 0) {
        count1Diff++;
        x -= 1;
    }
    while(count1Diff > 0) {
        result += 8;
        count1Diff--;
    }
    while(count8Diff > 0) {
        result -= 1;
        count8Diff--;
    }
    return result;
}

uint64_t TransformValue(uint64_t value) {
    uint64_t mask = 0x0000000000000001LLU, transfCoord = 0;
    int coordinates[8], iterator = 0;
    for(int x = 0; x < 64; x++) {
        if((value & mask)==1) {
            coordinates[iterator] = x;
            iterator++;
        }
        value >>= 1;
    }
    for(int i = 0; i < iterator; i++) {
        transfCoord |= Squares[TransformCoordinate(coordinates[i])];
    }
    return transfCoord;
}

// map<uint64_t,uint64_t> ** FileAttacks     = new map<uint64_t,uint64_t>*[64];
void HashFileAttacks(map<uint64_t,uint64_t> ** FileAttacks, map<uint64_t,uint64_t> ** RankAttacks) {
    for(int x = 0; x < 64; x++) {
        map<uint64_t,uint64_t>::iterator it;
        for(it = RankAttacks[x]->begin(); it != RankAttacks[x]->end(); it++) {
            FileAttacks[TransformCoordinate(x)]->insert(pair<uint64_t,uint64_t>(TransformValue(it->first), TransformValue(it->second)));
        }
    }
}

double GetDiagRowLength(int num) {
        double diagRowLength[] = {1,2,3,4,5,6,7,8,7,6,5,4,3,2,1};
    return diagRowLength[num];
}

// map<uint64_t,uint64_t> ** DiagAttacksA1H8 = new map<uint64_t,uint64_t>*[64];
void HashDiagAttacksA1H8(map<uint64_t,uint64_t> ** DiagAttacksA1H8) {
    uint64_t pos;
    int posNum;
    for(int x = 0; x < 15; x++) {
        for(int i = 0; (DiagBoardA1H8[x][i] != 0) && (i != 8); i++) {
            pos = DiagBoardA1H8[x][i];
            for(posNum = 0; (pos >> posNum) != 1; posNum++);
            for(int c = 0; c < pow(2,GetDiagRowLength(x)); c++ ) {
                uint64_t block = 0;
                for(int z = 0; (c >> z) != 0; z++ ) {
                    uint64_t firstBit = 0x0000000000000001LLU;
                    if((c >> z) & firstBit) {
                        block |= DiagBoardA1H8[x][z];
                    }
                }
                uint64_t down = 0, up = 0, value = 0, thisDiag = 0;
                int y;
                for( y = i; ; y--) {
                    down = DiagBoardA1H8[x][y];
                    if((value | DiagBoardA1H8[x][y])!= 0) {
                        value |= DiagBoardA1H8[x][y];
                    }
                    if(((down & block) != 0) || down == 0) {
                        if(down == 0) {
                            down = 0;
                        }
                        break;
                    }
                }
                for( y = i; ; y++) {
                    up = DiagBoardA1H8[x][y];
                    if((value | DiagBoardA1H8[x][y])!= 0) {
                        value |= DiagBoardA1H8[x][y];
                    }
                    if(((up & block) != 0) || ((up == 0) || (y == 8))) {
                        if(up == 0) {
                            up = 0;
                        }
                        break;
                    }
                }
                for(int y = 0; y <= DiagA1H8[posNum]; y++) {
                    thisDiag |= DiagBoardA1H8[x][y];
                }
                value &= thisDiag;
                DiagAttacksA1H8[posNum]->insert(pair<uint64_t,uint64_t>(pos^block,value));
            }
        }
    }
}

// map<uint64_t,uint64_t> ** DiagAttacksH1A8 = new map<uint64_t,uint64_t>*[64];
void HashDiagAttacksH1A8(map<uint64_t,uint64_t> ** DiagAttacksH1A8) {
    uint64_t pos;
    int posNum;
    for(int x = 0; x < 15; x++) {
        for(int i = 0; (DiagBoardH1A8[x][i] != 0) && (i != 8); i++) {
            pos = DiagBoardH1A8[x][i];
            for(posNum = 0; (pos >> posNum) != 1; posNum++) {
            }
            for(int c = 0; c < pow(2,GetDiagRowLength(x)); c++ ) {
                uint64_t block = 0;
                for(int z = 0; (c >> z) != 0; z++ ) {
                    uint64_t firstBit = 0x0000000000000001LLU;
                    if((c >> z) & firstBit) {
                        block |= DiagBoardH1A8[x][z];
                    }
                }
                uint64_t down = 0, up = 0, value = 0, thisDiag = 0;
                int y;
                for( y = i; ; y--) {
                    down = DiagBoardH1A8[x][y];
                    if((value | DiagBoardH1A8[x][y])!= 0) {
                        value |= DiagBoardH1A8[x][y];
                    }
                    if(((down & block) != 0) || down == 0) {
                        if(down == 0) {
                            down = 0;
                        }
                        break;
                    }
                }
                for( y = i; ; y++) {
                    up = DiagBoardH1A8[x][y];
                    if((value | DiagBoardH1A8[x][y])!= 0) {
                        value |= DiagBoardH1A8[x][y];
                    }
                    if(((up & block) != 0) || ((up == 0) || (y == 8))) {
                        if(up == 0) {
                            up = 0;
                        }
                        break;
                    }
                }
                for(int y = 0; y <= DiagH1A8[posNum]; y++) {
                    thisDiag |= DiagBoardH1A8[x][y];
                }
                value &= thisDiag;
                DiagAttacksH1A8[posNum]->insert(pair<uint64_t,uint64_t>(pos^block,value));
            }
        }
    }
}

void InitLookupTables( map<uint64_t,uint64_t> ** RankAttacks,
                         map<uint64_t,uint64_t> ** FileAttacks,
                         map<uint64_t,uint64_t> ** DiagAttacksA1H8,
                         map<uint64_t,uint64_t> ** DiagAttacksH1A8 ) {                   
        HashRankAttacks(RankAttacks);
        HashFileAttacks(FileAttacks, RankAttacks);
        HashDiagAttacksA1H8(DiagAttacksA1H8);
        HashDiagAttacksH1A8(DiagAttacksH1A8);
}
