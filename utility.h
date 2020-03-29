#include <iostream>
#include <string>
using namespace std;

#ifndef UTILITY_H
#define UTILITY_H


enum GameMode { GAME_MODE_NOT_SET = -1, TWO_PLAYERS_MANUAL, AI_HELPER_MODE, AI_OPPONENT_MODE };
enum AIMode {AI_MODE_NOT_SET = -1, AI_HINT, AI_AUTO};
enum ColorOrderMode {COLOR_ORDER_NOT_SET = -1, YOU_WHITE_FIRST, YOU_BLACK_FIRST, OPPONENT_WHITE_FIRST, OPPONENT_BLACK_FIRST};

enum PlayerTurn {TURN_NOT_DEFINED = -1, YOUR_TURN, OPPONENT_TURN};
enum PlayerColor {COLOR_NOT_DEFINED = -1, YOU_WHITE_COLOR, YOU_BLACK_COLOR};

enum GamePhrase {OPENING, MIDGAME, ENDGAME};

// Neighbor info (2D arr)
const string Neighbor[21][4] = {  {"02","03","07","  "}, // 01
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

const int NeighborCount[21] = {   3, // 01
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


#endif // !UTILITY_H


