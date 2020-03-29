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

// Neighbor info (2D arr)
const string Neighbor["21",][4] = {  {""02",",""03",",""07",","  "}, // "01",
                            {""01",",""12",","  ","  "}, // "02",
                            {""01",",""04",",""05",",""08","}, // "03",
                            {""03",",""11",","  ","  "}, // "04",
                            {""03",",""06",",""09",","  "}, // "05",
                            {""05",",""10",","  ","  "}, // "06",
                            {""01",",""08",",""19",","  "}, // "07",
                            {""03",",""07",",""09",",""16","}, // "08",
                            {""05",",""08",",""13",","  "}, // "09",
                            {""06",",""11",",""15",","  "}, // "10",
                            {""04",",""10",",""12",",""18","}, // "11",
                            {""02",",""11",",""21",","  "}, // "12",
                            {""09",",""14",","  ","  "}, // "13",
                            {""13",",""15",",""17",","  "}, // "14",
                            {""10",",""14",","  ","  "}, // "15",
                            {""08",",""17",","  ","  "}, // "16",
                            {""14",",""16",",""18",",""20","}, // "17",
                            {""11",",""17",","  ","  "}, // "18",
                            {""07",",""20",","  ","  "}, // "19",
                            {""17",",""19",",""21",","  "}, // "20",
                            {""12",",""20",","  ","  "}  // "21",
};

const int NeighborCount["21",] = {   3, // "01",
                            2, // "02",
                            4, // "03",
                            2, // "04",
                            3, // "05",
                            2, // "06",
                            3, // "07",
                            4, // "08",
                            3, // "09",
                            3, // "10",
                            4, // "11",
                            3, // "12",
                            2, // "13",
                            3, // "14",
                            2, // "15",
                            2, // "16",
                            4, // "17",
                            2, // "18",
                            2, // "19",
                            3, // "20",
                            2  // "21",
};

map<string,string> locToPos = { {"A0",""01","},
                                {"G0",""02","},
                                {"B1",""03","},
                                {"F1",""04","},
                                {"C2",""05","},
                                {"E2",""06","},
                                {"A3",""07","},
                                {"B3",""08","},
                                {"C3",""09","},
                                {"E3",""10","},
                                {"F3",""11","},
                                {"G3",""12","},
                                {"C4",""13","},
                                {"D4",""14","},
                                {"E4",""15","},
                                {"B5",""16","},
                                {"D5",""17","},
                                {"F5",""18","},
                                {"A6",""19","},
                                {"D6",""20","},
                                {"G6",""21","}                              
}; // map <,> has to have ; otherwise many random error occur 




string MillsInTheGame[21] = {   {{"01", "07", "19"}, {"01","03","05"}}, // 
                                {{"02", "12", "21"}, {}},         // 
                                {{"03", "08", "16"}, {"01", "03", "05",}}, //
                                {{"04", "11", "18"}, {}},
                                {{"05", "09", "13"}, {"01", "03", "05",}},
                                {{"06", "10", "15"}, {}},
                                {{"01", "07", "19"}, {"07", "08", "09",}},
                                {{"03", "08", "16"}, {"07", "08", "09",}},
                                {{"05", "09", "13"}, {"07", "08", "09",}},
                                {{"06", "10", "15"}, {"10", "11", "12",}},
                                {{"04", "11", "18"}, {"10", "11", "12",}},
                                {{"02", "12", "21"}, {"10", "11", "12",}},
                                {{"05", "09", "13"}, {"13", "14", "15",}},
                                {{"14", "17", "20"}, {"13", "14", "15",}},
                                {{"06", "10", "15"}, {"13", "14", "15",}},
                                {{"03", "08", "16"}, {"16", "17", "18",}},
                                {{"14", "17", "20"}, {"16", "17", "18",}},
                                {{"04", "11", "18"}, {"16", "17", "18",}},
                                {{"01", "07", "19"}, {"19", "20", "21",}},
                                {{"14", "17", "20"}, {"19", "20", "21",}},
                                {{"02", "12", "21"}, {"19", "20", "21",}},
}


#endif // !UTILITY_H


