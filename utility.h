#include <iostream>
#include <string>
#include <map>
#include <vector>
using namespace std;

#ifndef UTILITY_H
#define UTILITY_H



enum GameMode { GAME_MODE_NOT_SET = -1, TWO_PLAYERS_MANUAL, AI_HELPER_MODE, AI_OPPONENT_MODE };
enum AIMode {AI_MODE_NOT_SET = -1, AI_HINT, AI_AUTO};
enum ColorOrderMode {COLOR_ORDER_NOT_SET = -1, YOU_WHITE_FIRST, YOU_BLACK_FIRST, OPPONENT_WHITE_FIRST, OPPONENT_BLACK_FIRST};

enum PlayerTurn {TURN_NOT_DEFINED = -1, YOUR_TURN, OPPONENT_TURN};
enum PlayerColor {COLOR_NOT_DEFINED = -1, YOU_WHITE_COLOR, YOU_BLACK_COLOR};

enum GamePhrase {OPENING, MIDGAME, ENDGAME};

enum Winner {WINNER_NOT_SET = -1, YOU_WIN, OPPONENT_WIN, GAME_ENDS_IN_A_DRAW};
enum WinningSituation {WINNING_SITUATION_NOT_SET = -1, REMAINING_LESS_THAN_2_PIECES, NO_MORE_AVAILABLE_MOVES, REPEATED_WITHOUT_FORMING_A_MILL};

// Neighbor info (2D arr)
const string Neighbor[21][4] = {    {"02","03","07","  "}, // 01
                                    {"01","12","  ","  "}, // 02
                                    {"01","04","05","08"}, // 03
                                    {"03","11","  ","  "}, // 04
                                    {"03","06","09","  "}, // 05
                                    {"05","10","  ","  "}, // 06
                                    {"01","08","19","  "}, // 07
                                    {"03","07","09","16"}, // 08
                                    {"05","08","13","  "}, // 09
                                    {"06","11","15","  "}, // 10
                                    {"04","10","12","18"}, // 11
                                    {"02","11","21","  "}, // 12
                                    {"09","14","  ","  "}, // 13
                                    {"13","15","17","  "}, // 14
                                    {"10","14","  ","  "}, // 15
                                    {"08","17","  ","  "}, // 16
                                    {"14","16","18","20"}, // 17
                                    {"11","17","  ","  "}, // 18
                                    {"07","20","  ","  "}, // 19
                                    {"17","19","21","  "}, // 20
                                    {"12","20","  ","  "}  // 21
};

const int NeighborCount[21] = { 3, // 01
                                2, // 02
                                4, // 03
                                2, // 04
                                3, // 05
                                2, // 06
                                3, // 07
                                4, // 08
                                3, // 09
                                3, // 10
                                4, // 11
                                3, // 12
                                2, // 13
                                3, // 14
                                2, // 15
                                2, // 16
                                4, // 17
                                2, // 18
                                2, // 19
                                3, // 20
                                2  // 21
};

map<string,string> locToPos = { {"A0","01"},
                                {"G0","02"},
                                {"B1","03"},
                                {"F1","04"},
                                {"C2","05"},
                                {"E2","06"},
                                {"A3","07"},
                                {"B3","08"},
                                {"C3","09"},
                                {"E3","10"},
                                {"F3","11"},
                                {"G3","12"},
                                {"C4","13"},
                                {"D4","14"},
                                {"E4","15"},
                                {"B5","16"},
                                {"D5","17"},
                                {"F5","18"},
                                {"A6","19"},
                                {"D6","20"},
                                {"G6","21"}                              
}; // map <,> has to have ; otherwise many random error occur 


// Mill
//01
vector<string> mill01_1 = {"01", "07", "19"};
vector<string> mill01_2 = {"01", "03", "05"};
vector<vector<string>> mill01 = {mill01_1, mill01_2};

//02
vector<string> mill02_1 = {"02", "12", "21"};
vector<string> mill02_2;
vector<vector<string>> mill02 = {mill02_1, mill02_2};

//03
vector<string> mill03_1 = {"03", "08", "16"};
vector<string> mill03_2 = {"01", "03", "05"};
vector<vector<string>> mill03 = {mill03_1, mill03_2};

//04
vector<string> mill04_1 = {"04", "11", "18"};
vector<string> mill04_2;
vector<vector<string>> mill04 = {mill04_1, mill04_2};

//05
vector<string> mill05_1 = {"05", "09", "13"};
vector<string> mill05_2 = {"01", "03", "05"};
vector<vector<string>> mill05 = {mill05_1, mill05_2};

//06
vector<string> mill06_1 = {"06", "10", "15"};
vector<string> mill06_2;
vector<vector<string>> mill06 = {mill06_1, mill06_2};

//07
vector<string> mill07_1 = {"01", "07", "19"};
vector<string> mill07_2 = {"07", "08", "09"};
vector<vector<string>> mill07 = {mill07_1, mill07_2};

//08
vector<string> mill08_1 = {"03", "08", "16"};
vector<string> mill08_2 = {"07", "08", "09"};
vector<vector<string>> mill08 = {mill08_1, mill08_2};
          
//09
vector<string> mill09_1 = {"05", "09", "13"};
vector<string> mill09_2 = {"07", "08", "09"};
vector<vector<string>> mill09 = {mill09_1, mill09_2};

//10
vector<string> mill10_1 = {"06", "10", "15"};
vector<string> mill10_2 = {"10", "11", "12"};
vector<vector<string>> mill10 = {mill10_1, mill10_2};

//11
vector<string> mill11_1 = {"04", "11", "18"};
vector<string> mill11_2 = {"10", "11", "12"};
vector<vector<string>> mill11 = {mill11_1, mill11_2};

//12
vector<string> mill12_1 = {"02", "12", "21"};
vector<string> mill12_2 = {"10", "11", "12"};
vector<vector<string>> mill12 = {mill12_1, mill12_2};

//13
vector<string> mill13_1 = {"05", "09", "13"};
vector<string> mill13_2 = {"13", "14", "15"};
vector<vector<string>> mill13 = {mill13_1, mill13_2};

//14
vector<string> mill14_1 = {"14", "17", "20"};
vector<string> mill14_2 = {"13", "14", "15"};
vector<vector<string>> mill14 = {mill14_1, mill14_2};

//15
vector<string> mill15_1 = {"06", "10", "15"};
vector<string> mill15_2 = {"13", "14", "15"};
vector<vector<string>> mill15 = {mill15_1, mill15_2};

//16
vector<string> mill16_1 = {"03", "08", "16"};
vector<string> mill16_2 = {"16", "17", "18"};
vector<vector<string>> mill16 = {mill16_1, mill16_2};

//17
vector<string> mill17_1 = {"14", "17", "20"};
vector<string> mill17_2 = {"16", "17", "18"};
vector<vector<string>> mill17 = {mill17_1, mill17_2};

//18
vector<string> mill18_1 = {"04", "11", "18"};
vector<string> mill18_2 = {"16", "17", "18"};
vector<vector<string>> mill18 = {mill18_1, mill18_2};

//19
vector<string> mill19_1 = {"01", "07", "19"};
vector<string> mill19_2 = {"19", "20", "21"};
vector<vector<string>> mill19 = {mill19_1, mill19_2};

//20
vector<string> mill20_1 = {"14", "17", "20"};
vector<string> mill20_2 = {"19", "20", "21"};
vector<vector<string>> mill20 = {mill20_1, mill20_2};

//21
vector<string> mill21_1 = {"02", "12", "21"};
vector<string> mill21_2 = {"19", "20", "21"};
vector<vector<string>> mill21 = {mill21_1, mill21_2};

vector<vector<vector<string>>> Mill_In_Game = { mill01,
                                                mill02,
                                                mill03,
                                                mill04,
                                                mill05,
                                                mill06,
                                                mill07,
                                                mill08,
                                                mill09,
                                                mill10,
                                                mill11,
                                                mill12,
                                                mill13,
                                                mill14,
                                                mill15,
                                                mill16,
                                                mill17,
                                                mill18,
                                                mill19,
                                                mill20,
                                                mill21
};


#endif // !UTILITY_H


