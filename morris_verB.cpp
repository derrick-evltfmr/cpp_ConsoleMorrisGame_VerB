#include <stdio.h>
#include <stdlib.h>
#include <string>   // string, stoi()
#include <iostream>
using namespace std;

#include <vector>   // vector<T>
#include <cctype>   // isdigit()
#include <cstring>  // strlen()
#include <fstream>  // ofstream
#include <ctime>    // time(), time_t type, tm struct

#include "utility.h"

//========================================================//
// Main Menu                                              //
//========================================================//
void menu();
void programModeMenu();
ProgramMode program_mode = PROGRAM_MODE_NOT_SET;


//===================================================================================================================================================//
// ##################################################################################################################################################//
// ############################################################### GAME PART ########################################################################//
// ##################################################################################################################################################//
//===================================================================================================================================================//


//========================================================//
// Struct                                                 //
//========================================================//
struct placePiece {
    string ch;   // the string that to place on board
    int pos;     // position to place the piece
};

struct movePiece {
    placePiece removePiece;
    placePiece movetoPiece;
};

//========================================================//
// Functions Prototypes                                   //
//========================================================//
void gameModeMenu();
//void aiModeMenu();
void colorOrderModeMenu();

void game_run();

bool checkFormMill(string symbol[21], string place_piece);
bool checkFormMillForOther(string symbol[21], string place_piece);
bool checkIfAllPiecesInMill(string symbol[21]);

bool checkNoMoreAvailableMoves(string symbol[21]);
bool checkRepeatedMoves(string symbol[21],string removePiece_location, string movetoPiece_location);
void checkWinningSituation(string symbol[21]);  // I choose to not returning WinningSituation, 
                                                // just set the global variables and check them

vector<string> checkAvailableNeighbors(string symbol[21], string remove_piece);

struct placePiece generatePlacingPiece(string symbol[21]);
struct placePiece generateRemovingPiece(string symbol[21]);
struct movePiece generateMovingPieces(string symbol[21]);
struct movePiece generateFlyingPieces(string symbol[21]);

string inputValueHandler(string input);
char pieceCHtoWBXchar(string piece_ch);
string getStateOfTheTurn(string symbol[21]);


void createOutputFile();
void storeStateOfEachTurnInVecAndFile(string current_state_string, Action current_action, 
                                        placePiece place_info, placePiece remove_info, movePiece move_info);


string colorDecorator(string original_str);
void displayGameBoard(string symbol[21]);

void test();

//========================================================//
// Global Variables                                       //
//========================================================//
// handled by menus
GameMode game_mode = GAME_MODE_NOT_SET;
//AIMode ai_mode = AI_MODE_NOT_SET;
ColorOrderMode color_order_mode = COLOR_ORDER_NOT_SET;
PlayerTurn player_turn = TURN_NOT_DEFINED;
PlayerColor player_color = COLOR_NOT_DEFINED;

// handled by initializing
GamePhrase your_game_phrase = OPENING;
GamePhrase opponent_game_phrase = OPENING;

Winner game_winner = WINNER_NOT_SET;
WinningSituation game_winning_situation = WINNING_SITUATION_NOT_SET;
int notFormingMill_count = 0; // if 50 steps(can self-defined game rule) without forming a mill, 
                              // the game is draw (since a fair game, must be draw if no errors made)
                              // reset to 0 when (1). initialize the game (2). forming a mill

int num_your_pieces;
int num_opponent_pieces;
int num_your_removed_pieces;
int num_opponent_removed_pieces;

string your_this_remove;
string your_this_place;
string your_last_remove;
string your_last_place;
string opponent_this_remove;
string opponent_this_place;
string opponent_last_remove;
string opponent_last_place;

// state vector to store the input output
vector<string> game_states;

// output stream
ofstream ofs;

string current_state_string; // will be initialized in the function getStateOfTheTurn
Action current_action = ACTION_NOT_SET;
int states_count = 0;


int repeatedFormedMillCount = 0;


//========================================================//
// Extern Variables (if constant and included then may not needed)//
//========================================================//
extern const string Neighbor[][4];  // defined in utility.h
extern const int NeighborCount[21]; // defined in utility.h
extern map<string,string> locToPos; // defined in utility.h


//===================================================================================================================================================//
// ##################################################################################################################################################//
// ########################################################## PROJECT REQUIREMENT PART ##############################################################//
// ##################################################################################################################################################//
//===================================================================================================================================================//

//========================================================//
// Functions Prototypes (In the bottom of the code)       //
//========================================================//

void MiniMaxOpening();
void MiniMaxGame();
void ABOpening();
void ABGame();
void MiniMaxOpeningBlack();
void MiniMaxGameBlack();
void MiniMaxOpeningImproved();
void MiniMaxGameImproved();


//===================================================================================================================================================//
// ##################################################################################################################################################//
// ############################################################ MAIN PROGRAM ########################################################################//
// ##################################################################################################################################################//
//===================================================================================================================================================//

//========================================================//
// Main Program                                           //
//========================================================//
int main(int argc, char* argv[]){

    // set console size (windows and linux are different)
    system("echo '\e[8;32;100t'");

    
    // handle command line argument
    bool commandLineArgumentExists = true; // by default
    char ifn[255],ofn[255],dts[255]; 

    string inputFileName = "Input File Name Not Defined";
    string outputFileName = "Output File Name Not Defined";
    int depthNeededToBeSearched = -1; // "Depth Of The Tree Needed To Be Searched Not Defined"

    if (argc==4){ // 3 + 1

        // get command-line arguments
        strcpy(ifn,argv[1]); 
        strcpy(ofn,argv[2]); 
        strcpy(dts,argv[3]); 
        
        // check whether command-line arguments are valid
        char* ifnOK = strstr(ifn, ".txt");
        char* ofnOK = strstr(ofn, ".txt");
        int lengthOfDTS = strlen(dts); // dts is a char array, and strlen() take a c_string
        bool dtsOK = true;
        for (int i=0; i<lengthOfDTS; i++){
            if (isdigit(dts[i])==0) dtsOK = false; // if one character fails to be digit, then false
        }

        if (!ifnOK) cout << "Input File " << ifn << " does not have a .txt suffix. " << endl;
        if (!ofnOK) cout << "Output File " << ofn << " does not have a .txt suffix. " << endl;
        if (!dtsOK) cout << "Depth To Be Search " << dts << " is not a valid number. " << endl;
        if (!ifnOK || !ofnOK || !dtsOK) {
            cout << "Try Again !" << endl << endl;
            exit(0);
        }

        // if valid convert them to string
        string temp_ifn_str(ifn);
        string temp_ofn_str(ofn);
        string temp_dts_str(dts);
        
        inputFileName = temp_ifn_str;
        outputFileName = temp_ofn_str;
        depthNeededToBeSearched = stoi(temp_dts_str);

        cout << "Input File Name: " << inputFileName;
        cout << "Output File Name: "<< outputFileName;
        cout << "Depth to Search: " << depthNeededToBeSearched; // integer
    }
    else if (argc == 1) { // 0 + 1 (the program name is counted as one argument)

        commandLineArgumentExists = false; // do nothing but set commandLineArgumentExists to false
    
    }
    else {
        cout << endl << "You have inputted " << (argc-1) << " command-line arguments." << endl;
        cout << "The program accepts 3 command-line arguments (or 0 command-line argument) only." << endl;
        cout << "Please check again." << endl << endl;
        exit(0);
    }



    // start or restart
    string restartGame;
    playAgain:


    // show menu
    menu();


    // actions after choosing menu options

    if (program_mode == PLAY_MORRIS_GAME){


        // create outputfile
        createOutputFile();

        // run game
        game_run();

    }

    else {
        // if no command-line argument or restart
        if (commandLineArgumentExists == false){
            
            bool allInputOK = false;

            while (allInputOK==false){
                string temp_ifn_str;
                string temp_ofn_str;
                string temp_dts_str;

                cout << endl << "Enter the input file name: (with .txt file extension)" << endl;
                cin >> temp_ifn_str;
                size_t ifnFind = temp_ifn_str.find(".txt");
                if (ifnFind==string::npos) {
                    cout << "Input File [" << temp_ifn_str << "] does not have a .txt suffix. " << endl << "Try Again !" << endl << endl;
                    continue;
                }

                cout << endl << "Enter the output file name: (with .txt file extension)" << endl;
                cin >> temp_ofn_str;
                int ofnFind = temp_ofn_str.find(".txt");
                if (ofnFind==string::npos){
                    cout << "Output File [" << temp_ofn_str << "] does not have a .txt suffix. " << endl << "Try Again !" << endl << endl;
                    continue;
                }
                
                cout << endl << "Enter the depth you want to search: (number)" << endl;
                cin >> temp_dts_str;
                int lengthOfDTS = strlen(temp_dts_str.c_str());
                bool dtsOK = true;
                for (int i=0; i<lengthOfDTS; i++){
                    if (isdigit(temp_dts_str[i])==false) dtsOK = false; // if one character fails to be digit, then false
                }
                if (!dtsOK) {
                    cout << "Depth Of The Tree Needed To Be Search [" << dts << "] is not a valid number. " << endl<< "Try Again !" << endl << endl;
                    continue;
                }

                if (ifnFind!=string::npos && ofnFind!=string::npos && dtsOK) allInputOK = true;

            }
        }
        


        if (program_mode == MINIMAXOPENING){
            MiniMaxOpening();
        }

        else if (program_mode == MINIMAXGAME){
            MiniMaxGame();
        }

        else if (program_mode == ABOPENING){
            ABOpening();
        }
        
        else if (program_mode == ABGAME){
            ABGame();
        }

        else if (program_mode == MINIMAXOPENINGBLACK){
            MiniMaxOpeningBlack();
        }

        else if (program_mode == MINIMAXGAMEBLACK){
            MiniMaxGameBlack();
        }

        else if (program_mode == MINIMAXOPENINGIMPROVED){
            MiniMaxOpeningImproved();
        }

        else if (program_mode == MINIMAXGAMEIMPROVED){
            MiniMaxGameImproved();
        }


    }
    
    

    // print game result (IF THERE IS)
    // the game ends in a draw
    if (game_winner == GAME_ENDS_IN_A_DRAW){
        printf("\n\nThere has been 50 moves without forming any new mill.");
        printf("\n\n\033[1;43mTHE GAME ENDS IN A DRAW.\033[0m"); // YELLOW BACKGROUND
    }
    // the winner is
    if (game_winner == YOU_WIN){
        if (game_winning_situation == REMAINING_JUST_2_PIECES){
            printf("\n\nYour opponent has only 2 pieces left on the board.");
        }
        if (game_winning_situation == NO_MORE_AVAILABLE_MOVES){
            printf("\n\nYour opponent has no more available moves on the board.");
        }
        if (game_winning_situation == REPEATED_WITHOUT_FORMING_A_MILL){
            printf("\n\nYour opponent repeated his previous move in the game.");
        }
        
        printf("\n\n\033[1;42mYOU WIN THE GAME !!\033[0m"); // GREEN BACKGROUND
    }
    if (game_winner == OPPONENT_WIN){
        if (game_winning_situation == REMAINING_JUST_2_PIECES){
            printf("\n\nYou have only 2 pieces left on the board.");
        }
        if (game_winning_situation == NO_MORE_AVAILABLE_MOVES){
            printf("\n\nYou have no more available moves on the board.");
        }
        if (game_winning_situation == REPEATED_WITHOUT_FORMING_A_MILL){
            printf("\n\nYou repeated your previous move in the game.");
        }
        
        printf("\n\n\033[1;41mYOUR OPPONENT WINS THE GAME !!\033[0m"); // RED BACKGROUND
    }


    // after finish running the game, ask player whether to continue
	printf("\n\nIf you want to play to go back to the menu Enter 'Y'/'y', otherwise Enter 'Any other key' to Exit\n\n(Enter Your Choice ...)   ");
	cin >> restartGame;
	if(restartGame == "Y" || restartGame == "y"){
        system("clear");

        commandLineArgumentExists = false; // set to false, because we want new values, it will ask the user to enter the values

        program_mode = PROGRAM_MODE_NOT_SET; // without this, MAIN MENU WILL BE SKIPPED

        goto playAgain;
    }
    else exit(0);
}


//===================================================================================================================================================//
// ##################################################################################################################################################//
// ############################################################ GAME FUNCTIONS ######################################################################//
// ##################################################################################################################################################//
//===================================================================================================================================================//

//========================================================//
// run the game                                           //
//========================================================//

void game_run(){
    // ====================================================================================================================================//
    //        INITIALIZE / RESET BEFORE THE GAME STARTS
    // ====================================================================================================================================//
    // initialize count, place_info, symbol arr
    // int count = 0;
    struct placePiece place_info;
    struct placePiece remove_info;
    struct movePiece move_info;

    string symbol[21] = {"01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21"}; // reset 
    
    // initialize/reset num of pieces, game phrase
    num_your_pieces = 9;
    num_opponent_pieces = 9;
    num_your_removed_pieces = 0;
    num_opponent_removed_pieces = 0;

    your_game_phrase = OPENING;
    opponent_game_phrase = OPENING;

    game_winner = WINNER_NOT_SET;
    game_winning_situation = WINNING_SITUATION_NOT_SET;

    notFormingMill_count = 0;

    your_this_remove = "  ";
    your_this_place = "  ";
    your_last_remove = "  ";
    your_last_place = "  ";

    opponent_this_remove = "  ";
    opponent_this_place = "  ";
    opponent_last_remove = "  ";
    opponent_last_place = "  ";

    // reset the game_states vector
    game_states.clear(); // remove all elements, size = 0


    current_action = ACTION_NOT_SET;
    states_count = 0;

    // ======================================================================================================//




    // display the gameboard
    displayGameBoard(symbol);

    // ==================================================
    // THE MAIN GAME LOOP                              //
    // ==================================================
    // continueLoop point

    // ##################################################
    // ### Ask for input and place the piece ###
    // ##################################################
    continueLoop:

    // ================ //
    // YOUR TURN        //
    // ================ //
    if (player_turn == YOUR_TURN){
        //|| ============ ||
        //|| OPENING      ||
        //|| ============ ||
        if (your_game_phrase == OPENING) {
            // found the location that match the input
            place_info = generatePlacingPiece(symbol);
            // place piece on board
            symbol[place_info.pos] = place_info.ch;
            // update the free pieces
            num_your_pieces--;

            // ###################################################################
            // ### Store the state when action is made (game state is changed) ###
            // ###################################################################
            // at the end of the turn, the piece is set, before it goes back or ends, store the state
            current_state_string = getStateOfTheTurn(symbol);
            current_action = ACTION_PLACE;
            storeStateOfEachTurnInVecAndFile(current_state_string, current_action, place_info, remove_info, move_info);



            // get the last placed piece location
            string last_place_location = to_string(place_info.pos + 1);
            last_place_location = inputValueHandler(last_place_location); // handled if single digit without 0, e.g. "9" => "09"
            // check formed a mill
            bool formedAMill = checkFormMill(symbol, last_place_location);
            // if formed a mill, generate remove piece
            if (formedAMill){
                remove_info = generateRemovingPiece(symbol);
                // and remove piece on board
                symbol[remove_info.pos] = remove_info.ch;
                // number of your opponent removed piece +1 (because you remove oppoent's piece)
                num_opponent_removed_pieces++;
                
                // set the notFormingMill_count to -1, because later after the end of the turn will ++ to 0
                notFormingMill_count = -1;

                // ###################################################################
                // ### Store the state when action is made (game state is changed) ###
                // ###################################################################
                // at the end of the turn, the piece is set, before it goes back or ends, store the state
                current_state_string = getStateOfTheTurn(symbol);
                current_action = ACTION_REMOVE;
                storeStateOfEachTurnInVecAndFile(current_state_string, current_action, place_info, remove_info, move_info);
            
            }
            
        }

        //|| ============ ||
        //|| MIDGAME      ||
        //|| ============ ||
        if (your_game_phrase == MIDGAME) {

            // ##########################################################
            // # THE FIRST THING TO CHECK IN MIDGAME is,                #
            // # whether there are still available moves for the pieces #
            // ##########################################################
            bool noMoreAvailableMoves = checkNoMoreAvailableMoves(symbol);
            // if no more available moves
            if (noMoreAvailableMoves) {
                // it's your turn, but you don't have any available moves, so you lose
                game_winner = OPPONENT_WIN;
                game_winning_situation = NO_MORE_AVAILABLE_MOVES;
                // end the run() function

                // BEFORE RETURN THE FUNCTION, AS IT STOPPED IN THE MIDDLE OF THE TURN
                // ##################################################
                // ### Display the game board after making a move ###
                // ##################################################
                system("clear");
                
                // display the gameboard
                displayGameBoard(symbol);

                return;
            }
            // ##########################################################

            // check whether the input is valid[neighbor+empty] or not, if valid, generate two infos
            move_info = generateMovingPieces(symbol);
            // remove piece on board
            symbol[move_info.removePiece.pos] = move_info.removePiece.ch;
            // place piece on board
            symbol[move_info.movetoPiece.pos] = move_info.movetoPiece.ch;

            // ###################################################################
            // ### Store the state when action is made (game state is changed) ###
            // ###################################################################
            // at the end of the turn, the piece is set, before it goes back or ends, store the state
            current_state_string = getStateOfTheTurn(symbol);
            current_action = ACTION_MOVE;
            storeStateOfEachTurnInVecAndFile(current_state_string, current_action, place_info, remove_info, move_info);


            // get the last placed piece location
            string last_place_location = to_string(move_info.movetoPiece.pos + 1);
            last_place_location = inputValueHandler(last_place_location); // handled if single digit without 0, e.g. "9" => "09"
            // check formed a mill
            bool formedAMill = checkFormMill(symbol, last_place_location);
            // if formed a mill, generate remove piece
            if (formedAMill){
                remove_info = generateRemovingPiece(symbol);
                // and remove piece on board
                symbol[remove_info.pos] = remove_info.ch;
                // number of your opponent removed piece +1 (because you remove oppoent's piece)
                num_opponent_removed_pieces++;
                
                // set the notFormingMill_count to -1, because later after the end of the turn will ++ to 0
                notFormingMill_count = -1;

                // ###################################################################
                // ### Store the state when action is made (game state is changed) ###
                // ###################################################################
                // at the end of the turn, the piece is set, before it goes back or ends, store the state
                current_state_string = getStateOfTheTurn(symbol);
                current_action = ACTION_REMOVE;
                storeStateOfEachTurnInVecAndFile(current_state_string, current_action, place_info, remove_info, move_info);
            }

            // ##########################################################
            // # after checking if formed a mill, the next thing to     #
            // # is whether the player made repeated moves              #
            // ##########################################################

            // check if your current moves are the same as your last moves
            // first, get the current removePiece_location and movetoPiece_location
            string removePiece_location = to_string(move_info.removePiece.pos + 1);
            removePiece_location = inputValueHandler(removePiece_location);
            string movetoPiece_location = to_string(move_info.movetoPiece.pos + 1);
            movetoPiece_location = inputValueHandler(movetoPiece_location);

            // check with the last moves
            // last moves -> this moves -> current moves
            // we actually compare the current moves with last moves because we compare before updating

            bool repeatedMoves = checkRepeatedMoves(symbol, removePiece_location, movetoPiece_location);
            
            if (repeatedMoves && repeatedFormedMillCount!=1) repeatedFormedMillCount++; // default -1, the first time 0, next time 1

            // if repeatedMoves and not formedAMill and not the exception 1 turn after repeated (first time repeated=true is 0, next time repeated=true is 1)
            if (repeatedMoves && !formedAMill && repeatedFormedMillCount!=1) {
                // it's your turn, but you made repeated moves, so you lose
                game_winner = OPPONENT_WIN;
                game_winning_situation = REPEATED_WITHOUT_FORMING_A_MILL;
                // end the run() function

                // BEFORE RETURN THE FUNCTION, AS IT STOPPED IN THE MIDDLE OF THE TURN
                // ##################################################
                // ### Display the game board after making a move ###
                // ##################################################
                system("clear");
                
                // display the gameboard
                displayGameBoard(symbol);

                return;
            }

            // if games not end
            if (repeatedFormedMillCount == 1) repeatedFormedMillCount = -1; // reset to -1 (initial situation)

            // otherwise, update the lastmoves and thismoves 
            // (the first time last will be assigned to "  " because the last moves were in OPENING, which we don't care)
            
            // update last moves <= this moves 
            your_last_remove = your_this_remove;
            your_last_place = your_this_place; 
            // update this moves <= current moves
            your_this_remove = removePiece_location;
            your_this_place = movetoPiece_location; 

        }

        //|| ============ ||
        //|| ENDGAME      ||
        //|| ============ ||
        if (your_game_phrase == ENDGAME) {
            // check whether the input is valid[neighbor+empty] or not, if valid, generate two infos
            move_info = generateFlyingPieces(symbol);
            // remove piece on board
            symbol[move_info.removePiece.pos] = move_info.removePiece.ch;
            // place piece on board
            symbol[move_info.movetoPiece.pos] = move_info.movetoPiece.ch;


            // ###################################################################
            // ### Store the state when action is made (game state is changed) ###
            // ###################################################################
            // at the end of the turn, the piece is set, before it goes back or ends, store the state
            current_state_string = getStateOfTheTurn(symbol);
            current_action = ACTION_FLY;
            storeStateOfEachTurnInVecAndFile(current_state_string, current_action, place_info, remove_info, move_info);


            // get the last placed piece location
            string last_place_location = to_string(move_info.movetoPiece.pos + 1);
            last_place_location = inputValueHandler(last_place_location); // handled if single digit without 0, e.g. "9" => "09"
            // check formed a mill
            bool formedAMill = checkFormMill(symbol, last_place_location);
            // if formed a mill, generate remove piece
            if (formedAMill){
                remove_info = generateRemovingPiece(symbol);
                // and remove piece on board
                symbol[remove_info.pos] = remove_info.ch;
                // number of your opponent removed piece +1 (because you remove oppoent's piece)
                num_opponent_removed_pieces++;
                
                // set the notFormingMill_count to -1, because later after the end of the turn will ++ to 0
                notFormingMill_count = -1;


                // ###################################################################
                // ### Store the state when action is made (game state is changed) ###
                // ###################################################################
                // at the end of the turn, the piece is set, before it goes back or ends, store the state
                current_state_string = getStateOfTheTurn(symbol);
                current_action = ACTION_REMOVE;
                storeStateOfEachTurnInVecAndFile(current_state_string, current_action, place_info, remove_info, move_info);

            }

            // ##########################################################
            // # after checking if formed a mill, the next thing to     #
            // # is whether the player made repeated moves              #
            // ##########################################################

            // check if your current moves are the same as your last moves
            // first, get the current removePiece_location and movetoPiece_location
            string removePiece_location = to_string(move_info.removePiece.pos + 1);
            removePiece_location = inputValueHandler(removePiece_location);
            string movetoPiece_location = to_string(move_info.movetoPiece.pos + 1);
            movetoPiece_location = inputValueHandler(movetoPiece_location);

            // check with the last moves
            // last moves -> this moves -> current moves
            // we actually compare the current moves with last moves because we compare before updating

            bool repeatedMoves = checkRepeatedMoves(symbol, removePiece_location, movetoPiece_location);

            if (repeatedMoves && repeatedFormedMillCount!=1) repeatedFormedMillCount++; // default -1, the first time 0, next time 1

            // if repeatedMoves and not formedAMill and not the exception 1 turn after repeated (first time repeated=true is 0, next time repeated=true is 1)
            if (repeatedMoves && !formedAMill && repeatedFormedMillCount!=1) {
                // it's your turn, but you made repeated moves, so you lose
                game_winner = OPPONENT_WIN;
                game_winning_situation = REPEATED_WITHOUT_FORMING_A_MILL;
                // end the run() function

                // BEFORE RETURN THE FUNCTION, AS IT STOPPED IN THE MIDDLE OF THE TURN
                // ##################################################
                // ### Display the game board after making a move ###
                // ##################################################
                system("clear");
                
                // display the gameboard
                displayGameBoard(symbol);

                return;
            }

            // if games not end
            if (repeatedFormedMillCount == 1) repeatedFormedMillCount = -1; // reset to -1 (initial situation)

            // otherwise, update the lastmoves and thismoves 
            // (the first time last will be assigned to "  " because the last moves were in OPENING, which we don't care)
            
            // update last moves <= this moves 
            your_last_remove = your_this_remove;
            your_last_place = your_this_place; 
            // update this moves <= current moves
            your_this_remove = removePiece_location;
            your_this_place = movetoPiece_location; 

        }

    }

    // ================ //
    // OPPONENT TURN    //
    // ================ //
    else if (player_turn == OPPONENT_TURN) {
        //|| ============ ||
        //|| OPENING      ||
        //|| ============ ||
        if (opponent_game_phrase == OPENING) {
            // found the location that match the input
            place_info = generatePlacingPiece(symbol);
            // place piece on board
            symbol[place_info.pos] = place_info.ch;
            // update the free pieces
            num_opponent_pieces--;

            
            // ###################################################################
            // ### Store the state when action is made (game state is changed) ###
            // ###################################################################
            // at the end of the turn, the piece is set, before it goes back or ends, store the state
            current_state_string = getStateOfTheTurn(symbol);
            current_action = ACTION_PLACE;
            storeStateOfEachTurnInVecAndFile(current_state_string, current_action, place_info, remove_info, move_info);


            // get the last placed piece location
            string last_place_location = to_string(place_info.pos + 1);
            last_place_location = inputValueHandler(last_place_location); // handled if single digit without 0, e.g. "9" => "09"
            // check formed a mill
            bool formedAMill = checkFormMill(symbol, last_place_location);
            // if formed a mill, generate remove piece
            if (formedAMill){
                remove_info = generateRemovingPiece(symbol);
                // and remove piece on board
                symbol[remove_info.pos] = remove_info.ch;
                // number of your removed piece +1 (because your opponent remove your piece)
                num_your_removed_pieces++;

                // set the notFormingMill_count to -1, because later after the end of the turn will ++ to 0
                notFormingMill_count = -1;


                // ###################################################################
                // ### Store the state when action is made (game state is changed) ###
                // ###################################################################
                // at the end of the turn, the piece is set, before it goes back or ends, store the state
                current_state_string = getStateOfTheTurn(symbol);
                current_action = ACTION_REMOVE;
                storeStateOfEachTurnInVecAndFile(current_state_string, current_action, place_info, remove_info, move_info);

            }
        }

        
        //|| ============ ||
        //|| MIDGAME      ||
        //|| ============ ||
        if (opponent_game_phrase == MIDGAME) {

            // ##########################################################
            // # THE FIRST THING TO CHECK IN MIDGAME is,                #
            // # whether there are still available moves for the pieces #
            // ##########################################################
            bool noMoreAvailableMoves = checkNoMoreAvailableMoves(symbol);
            // if no more available moves
            if (noMoreAvailableMoves) {
                // it's your opponent's turn, but he does't have any available moves, so opponent lose
                game_winner = YOU_WIN;
                game_winning_situation = NO_MORE_AVAILABLE_MOVES;
                // end the run() function

                // BEFORE RETURN THE FUNCTION, AS IT STOPPED IN THE MIDDLE OF THE TURN
                // ##################################################
                // ### Display the game board after making a move ###
                // ##################################################
                system("clear");
                
                // display the gameboard
                displayGameBoard(symbol);

                return;
            }
            // ##########################################################


            // check whether the input is valid[neighbor+empty] or not, if valid, generate two infos
            move_info = generateMovingPieces(symbol);
            // remove piece on board
            symbol[move_info.removePiece.pos] = move_info.removePiece.ch;
            // place piece on board
            symbol[move_info.movetoPiece.pos] = move_info.movetoPiece.ch;

            // ###################################################################
            // ### Store the state when action is made (game state is changed) ###
            // ###################################################################
            // at the end of the turn, the piece is set, before it goes back or ends, store the state
            current_state_string = getStateOfTheTurn(symbol);
            current_action = ACTION_MOVE;
            storeStateOfEachTurnInVecAndFile(current_state_string, current_action, place_info, remove_info, move_info);


            // get the last placed piece location
            string last_place_location = to_string(move_info.movetoPiece.pos + 1);
            last_place_location = inputValueHandler(last_place_location); // handled if single digit without 0, e.g. "9" => "09"
            // check formed a mill
            bool formedAMill = checkFormMill(symbol, last_place_location);
            // if formed a mill, generate remove piece
            if (formedAMill){
                remove_info = generateRemovingPiece(symbol);
                // and remove piece on board
                symbol[remove_info.pos] = remove_info.ch;
                // number of your removed piece +1 (because your opponent remove your piece)
                num_your_removed_pieces++;

                // set the notFormingMill_count to -1, because later after the end of the turn will ++ to 0
                notFormingMill_count = -1;

                // ###################################################################
                // ### Store the state when action is made (game state is changed) ###
                // ###################################################################
                // at the end of the turn, the piece is set, before it goes back or ends, store the state
                current_state_string = getStateOfTheTurn(symbol);
                current_action = ACTION_REMOVE;
                storeStateOfEachTurnInVecAndFile(current_state_string, current_action, place_info, remove_info, move_info);

            }

            // ##########################################################
            // # after checking if formed a mill, the next thing to     #
            // # is whether the player made repeated moves              #
            // ##########################################################

            // check if oppoent's current moves are the same as opponent's last moves
            // first, get the current removePiece_location and movetoPiece_location
            string removePiece_location = to_string(move_info.removePiece.pos + 1);
            removePiece_location = inputValueHandler(removePiece_location);
            string movetoPiece_location = to_string(move_info.movetoPiece.pos + 1);
            movetoPiece_location = inputValueHandler(movetoPiece_location);

            // check with the last moves
            // last moves -> this moves -> current moves
            // we actually compare the current moves with last moves because we compare before updating

            bool repeatedMoves = checkRepeatedMoves(symbol, removePiece_location, movetoPiece_location);

            if (repeatedMoves && repeatedFormedMillCount!=1) repeatedFormedMillCount++; // default -1, the first time 0, next time 1

            // if repeatedMoves and not formedAMill and not the exception 1 turn after repeated (first time repeated=true is 0, next time repeated=true is 1)
            if (repeatedMoves && !formedAMill && repeatedFormedMillCount!=1) {
                // it's opponent turn, but he made repeated moves, so he loses
                game_winner = YOU_WIN;
                game_winning_situation = REPEATED_WITHOUT_FORMING_A_MILL;
                // end the run() function

                // BEFORE RETURN THE FUNCTION, AS IT STOPPED IN THE MIDDLE OF THE TURN
                // ##################################################
                // ### Display the game board after making a move ###
                // ##################################################
                system("clear");
                
                // display the gameboard
                displayGameBoard(symbol);

                return;
            }
            
            // if games not end
            if (repeatedFormedMillCount == 1) repeatedFormedMillCount = -1; // reset to -1 (initial situation)

            // otherwise, update the lastmoves and thismoves 
            // (the first time last will be assigned to "  " because the last moves were in OPENING, which we don't care)
            
            // update last moves <= this moves 
            opponent_last_remove = opponent_this_remove;
            opponent_last_place = opponent_this_place; 
            // update this moves <= current moves
            opponent_this_remove = removePiece_location;
            opponent_this_place = movetoPiece_location; 


        }

        //|| ============ ||
        //|| ENDGAME      ||
        //|| ============ ||
        if (opponent_game_phrase == ENDGAME) {
            // check whether the input is valid[neighbor+empty] or not, if valid, generate two infos
            move_info = generateFlyingPieces(symbol);
            // remove piece on board
            symbol[move_info.removePiece.pos] = move_info.removePiece.ch;
            // place piece on board
            symbol[move_info.movetoPiece.pos] = move_info.movetoPiece.ch;


            // ###################################################################
            // ### Store the state when action is made (game state is changed) ###
            // ###################################################################
            // at the end of the turn, the piece is set, before it goes back or ends, store the state
            current_state_string = getStateOfTheTurn(symbol);
            current_action = ACTION_FLY;
            storeStateOfEachTurnInVecAndFile(current_state_string, current_action, place_info, remove_info, move_info);


            // get the last placed piece location
            string last_place_location = to_string(move_info.movetoPiece.pos + 1);
            last_place_location = inputValueHandler(last_place_location); // handled if single digit without 0, e.g. "9" => "09"
            // check formed a mill
            bool formedAMill = checkFormMill(symbol, last_place_location);
            // if formed a mill, generate remove piece
            if (formedAMill){
                remove_info = generateRemovingPiece(symbol);
                // and remove piece on board
                symbol[remove_info.pos] = remove_info.ch;
                // number of your removed piece +1 (because your opponent remove your piece)
                num_your_removed_pieces++;

                // set the notFormingMill_count to -1, because later after the end of the turn will ++ to 0
                notFormingMill_count = -1;


                // ###################################################################
                // ### Store the state when action is made (game state is changed) ###
                // ###################################################################
                // at the end of the turn, the piece is set, before it goes back or ends, store the state
                current_state_string = getStateOfTheTurn(symbol);
                current_action = ACTION_REMOVE;
                storeStateOfEachTurnInVecAndFile(current_state_string, current_action, place_info, remove_info, move_info);

            }


            // ##########################################################
            // # after checking if formed a mill, the next thing to     #
            // # is whether the player made repeated moves              #
            // ##########################################################

            // check if oppoent's current moves are the same as opponent's last moves
            // first, get the current removePiece_location and movetoPiece_location
            string removePiece_location = to_string(move_info.removePiece.pos + 1);
            removePiece_location = inputValueHandler(removePiece_location);
            string movetoPiece_location = to_string(move_info.movetoPiece.pos + 1);
            movetoPiece_location = inputValueHandler(movetoPiece_location);

            // check with the last moves
            // last moves -> this moves -> current moves
            // we actually compare the current moves with last moves because we compare before updating

            bool repeatedMoves = checkRepeatedMoves(symbol, removePiece_location, movetoPiece_location);

            if (repeatedMoves && repeatedFormedMillCount!=1) repeatedFormedMillCount++; // default -1, the first time 0, next time 1

            // if repeatedMoves and not formedAMill and not the exception 1 turn after repeated (first time repeated=true is 0, next time repeated=true is 1)
            if (repeatedMoves && !formedAMill && repeatedFormedMillCount!=1) {
                // it's opponent turn, but he made repeated moves, so he loses
                game_winner = YOU_WIN;
                game_winning_situation = REPEATED_WITHOUT_FORMING_A_MILL;
                // end the run() function

                // BEFORE RETURN THE FUNCTION, AS IT STOPPED IN THE MIDDLE OF THE TURN
                // ##################################################
                // ### Display the game board after making a move ###
                // ##################################################
                system("clear");
                
                // display the gameboard
                displayGameBoard(symbol);

                return;
            }
            
            // if games not end
            if (repeatedFormedMillCount == 1) repeatedFormedMillCount = -1; // reset to -1 (initial situation)

            // otherwise, update the lastmoves and thismoves 
            // (the first time last will be assigned to "  " because the last moves were in OPENING, which we don't care)
            
            // update last moves <= this moves 
            opponent_last_remove = opponent_this_remove;
            opponent_last_place = opponent_this_place; 
            // update this moves <= current moves
            opponent_this_remove = removePiece_location;
            opponent_this_place = movetoPiece_location; 

        }


    }

    // ##################################################
    // ### Process the game after making the move     ###
    // ##################################################
    // switch turn after placing the piece
    if (player_turn == YOUR_TURN)
        player_turn = OPPONENT_TURN;
    else if (player_turn == OPPONENT_TURN)
        player_turn = YOUR_TURN;

    // switch game phrase
    int num_your_remaining_pieces = 9 - num_your_removed_pieces; 
    int num_opponent_remaining_pieces = 9 - num_opponent_removed_pieces; 
    
    if (your_game_phrase == OPENING){
        if (num_your_pieces == 0) your_game_phrase = MIDGAME;
        if (num_your_remaining_pieces == 3) your_game_phrase = ENDGAME;
    }
    if (your_game_phrase == MIDGAME){
        if (num_your_remaining_pieces == 3) your_game_phrase = ENDGAME;
    }

    if (opponent_game_phrase == OPENING){
        if (num_opponent_pieces == 0) opponent_game_phrase = MIDGAME;
        if (num_opponent_remaining_pieces == 3) opponent_game_phrase = ENDGAME;
    }
    if (opponent_game_phrase == MIDGAME){
        if (num_opponent_remaining_pieces == 3) opponent_game_phrase = ENDGAME;
    }


    // ##################################################
    // ### Display the game board after making a move ###
    // ##################################################
    system("clear");
    
    // display the gameboard
    displayGameBoard(symbol);


    // ##################################################
    // ### Check winning situation / draw situation   ###
    // ##################################################
    // check Winning Situation - will change the value of game_winner and game_winning_situation if any of the conditions matches
    checkWinningSituation(symbol);

    // if game_winner is set to something else (!=NOT SET means SET)
    if(game_winner!=WINNER_NOT_SET){
        // do nothing, not go back to continueLoop, so the function run() will end
    }

    // if game_winner is still not set, turn + 1, check if draws and continueLoop
    else{
        notFormingMill_count++;

        // if notFormingMill for 50 turns, set to a draw, and since not going back to continueLoop, so the function run() will end
        if (notFormingMill_count >= 50){
            game_winner = GAME_ENDS_IN_A_DRAW;
        }
        // if not exceeding 50 turns, continue
        else {
            goto continueLoop;
        }
    }

}

//========================================================//
// checkFormMill                                          //
//========================================================//
bool checkFormMill(string symbol[21], string place_piece){

    bool formedAMill = false;

    int place_piece_index = stoi(place_piece) - 1;

    int fillCount; // count whether 3 mill locations occupied

    // check through each mill set
    for (int i=0; i<Mill_In_Game[place_piece_index].size();i++){           //mill01 -> {mill01_1, mill01_2}
        
        fillCount = 0; // reset fillCount = 0
        
        // check through each mill element
        for (int j=0; j<Mill_In_Game[place_piece_index][i].size();j++){    //mill01_1 -> {"01", "07", "19"} / mill01_2 -> {"01", "03", "05"}
            // cout << Mill_In_Game[place_piece_index][i][j] << " ";

            // check if the symbol[mill_set_index] is occupied
            int mill_set_index = stoi(Mill_In_Game[place_piece_index][i][j]) - 1;
            
            string occupied_ch;
            if (player_turn == YOUR_TURN && player_color == YOU_WHITE_COLOR) occupied_ch = " W";
            else if (player_turn == YOUR_TURN && player_color == YOU_BLACK_COLOR) occupied_ch = " B";
            else if (player_turn == OPPONENT_TURN && player_color == YOU_BLACK_COLOR) occupied_ch = " W";
            else if (player_turn == OPPONENT_TURN && player_color == YOU_WHITE_COLOR) occupied_ch = " B";

            // if match and occupied, fillCount ++
            if (symbol[mill_set_index]==occupied_ch){
                fillCount++;
            }

        }
        
        // if found 3 mill locations occupied, a mill was formed
        if (fillCount == 3){
            formedAMill = true;
            break;
        }

    }

    return formedAMill;

    
}


//========================================================//
// checkFormMillForOther (opposite occupied_ch)           //
//========================================================//
bool checkFormMillForOther(string symbol[21], string place_piece){

    bool formedAMill = false;

    int place_piece_index = stoi(place_piece) - 1;

    int fillCount; // count whether 3 mill locations occupied

    // check through each mill set
    for (int i=0; i<Mill_In_Game[place_piece_index].size();i++){           //mill01 -> {mill01_1, mill01_2}
        
        fillCount = 0; // reset fillCount = 0
        
        // check through each mill element
        for (int j=0; j<Mill_In_Game[place_piece_index][i].size();j++){    //mill01_1 -> {"01", "07", "19"} / mill01_2 -> {"01", "03", "05"}
            // cout << Mill_In_Game[place_piece_index][i][j] << " ";

            // check if the symbol[mill_set_index] is occupied
            int mill_set_index = stoi(Mill_In_Game[place_piece_index][i][j]) - 1;
            
            string occupied_ch;
            if (player_turn == YOUR_TURN && player_color == YOU_WHITE_COLOR) occupied_ch = " B";
            else if (player_turn == YOUR_TURN && player_color == YOU_BLACK_COLOR) occupied_ch = " W";
            else if (player_turn == OPPONENT_TURN && player_color == YOU_BLACK_COLOR) occupied_ch = " B";
            else if (player_turn == OPPONENT_TURN && player_color == YOU_WHITE_COLOR) occupied_ch = " W";

            // if match and occupied, fillCount ++
            if (symbol[mill_set_index]==occupied_ch){
                fillCount++;
            }

        }
        
        // if found 3 mill locations occupied, a mill was formed
        if (fillCount == 3){
            formedAMill = true;
            break;
        }

    }

    return formedAMill;

    
}

//========================================================//
// checkIfAllPiecesInMill (opposite occupied_ch)           //
//========================================================//
bool checkIfAllPiecesInMill(string symbol[21]){
    bool allPiecesInMill = false;
    int pieceInMillCount = 0;

    int num_of_remaining_pieces;
    // note that we want the value of the other, so it is the opposite
    if (player_turn==YOUR_TURN){
        num_of_remaining_pieces = num_opponent_pieces - num_opponent_removed_pieces;
    }
    else if (player_turn==OPPONENT_TURN){
        num_of_remaining_pieces = num_your_pieces - num_your_removed_pieces;
    }
    else printf("ERROR: in checkIfAllPieces, player_turn is not YOUR or OPPONENT's turn.\n");

    string occupied_ch; // this is reversed as checkFormMillForOther, becuase you want to check the other's pieces
    if (player_turn == YOUR_TURN && player_color == YOU_WHITE_COLOR) occupied_ch = " B";
    else if (player_turn == YOUR_TURN && player_color == YOU_BLACK_COLOR) occupied_ch = " W";
    else if (player_turn == OPPONENT_TURN && player_color == YOU_BLACK_COLOR) occupied_ch = " B";
    else if (player_turn == OPPONENT_TURN && player_color == YOU_WHITE_COLOR) occupied_ch = " W";

    vector<string> the_other_pieces;

    // find all the other's pieces
    for(int i=0;i<21;i++){

        if(symbol[i]==occupied_ch){   // if the board occupied equal the color piece we want to find
            
            // we don't need break because we want to find all
            int the_other_pieces_index = i;
            string the_other_piece_location = to_string(the_other_pieces_index+1);
            the_other_piece_location = inputValueHandler(the_other_piece_location); // handled the single-digit number, e.g. "9"->"09"
            // if found, add to the vector
            the_other_pieces.push_back(the_other_piece_location);
        }

    }

    // for each other's piece
    for (int i=0;i<the_other_pieces.size();i++){ // index is always less than size by 1 because starting from 0
        bool pieceInAMill = false;

        pieceInAMill = checkFormMillForOther(symbol, the_other_pieces[i]); // for the opposite other color, assume the_other_pieces[i] is to remove

        if (pieceInAMill) pieceInMillCount++;
    }

    // if all remaining pieces are in mill(s), then true
    if (pieceInMillCount == num_of_remaining_pieces){
        allPiecesInMill = true;
    }

    return allPiecesInMill;
}

//========================================================//
// checkNoMoreAvailableMoves                              //
//========================================================//
bool checkNoMoreAvailableMoves(string symbol[21]){
    bool noMoreAvailableMoves = false;
    int noAvailableNeighbor_count = 0;

    string occupied_ch; // not reversed, exactly, if your turn and you are white then white
    if (player_turn == YOUR_TURN && player_color == YOU_WHITE_COLOR) occupied_ch = " W";
    else if (player_turn == YOUR_TURN && player_color == YOU_BLACK_COLOR) occupied_ch = " B";
    else if (player_turn == OPPONENT_TURN && player_color == YOU_BLACK_COLOR) occupied_ch = " W";
    else if (player_turn == OPPONENT_TURN && player_color == YOU_WHITE_COLOR) occupied_ch = " B";

    vector<string> the_player_pieces;

    // find all the player's pieces
    for(int i=0;i<21;i++){

        if(symbol[i]==occupied_ch){   // if the board occupied equal the color piece we want to find
            
            // we don't need break because we want to find all
            int the_player_pieces_index = i;
            string the_player_piece_location = to_string(the_player_pieces_index+1);
            the_player_piece_location = inputValueHandler(the_player_piece_location); // handled the single-digit number, e.g. "9"->"09"
            // if found, add to the vector
            the_player_pieces.push_back(the_player_piece_location);
        }

    }

    // for each player's piece
    for (int i=0;i<the_player_pieces.size();i++){ // index is always less than size by 1 because starting from 0

        // check if there is any available and free neighbors
        vector<string> available_neighbors;
        available_neighbors = checkAvailableNeighbors(symbol, the_player_pieces[i]); // put the player pieces [i] to check their neighbors

        // if for this player's piece no available neighbors
        if (available_neighbors.size() == 0) {
            noAvailableNeighbor_count++;
        }

    }

    if (noAvailableNeighbor_count == the_player_pieces.size()){ // can use the_player_pieces.size(), or num_of_remaining, but just too lazy to check
        noMoreAvailableMoves = true;
    }

    return noMoreAvailableMoves;
}

//========================================================//
// checkRepeatedMoves (MIDGAME and ENDGAME)               //
//========================================================//
bool checkRepeatedMoves(string symbol[21], string removePiece_location, string movetoPiece_location){
    bool repeatedMoves = false;

    if (player_turn == YOUR_TURN){
        if (removePiece_location == your_last_remove && movetoPiece_location == your_last_place){
            repeatedMoves = true;
        }
    }

    else if (player_turn == OPPONENT_TURN){
        if (removePiece_location == opponent_last_remove && movetoPiece_location == opponent_last_place){
            repeatedMoves = true;
        }
    }

    return repeatedMoves;
}


//========================================================//
// checkWinningSituation                                  //
//========================================================//
void checkWinningSituation(string symbol[21]){
    // get the updatest remainingpieces numbers
    int num_your_remaining_pieces = 9 - num_your_removed_pieces; 
    int num_opponent_remaining_pieces = 9 - num_opponent_removed_pieces; 

    // CHECK IF WINNING SITUATION HAPPENS
    // (1) any player remaining pieces less than 3 (just 2 pieces left)
    if (num_your_remaining_pieces == 2){
        game_winner = OPPONENT_WIN;
        game_winning_situation = REMAINING_JUST_2_PIECES;
        return;
    }
    if (num_opponent_remaining_pieces == 2){
        game_winner = YOU_WIN;
        game_winning_situation = REMAINING_JUST_2_PIECES;
        return;
    }

    // (2) no more available moves in MIDGAME (because won't happen in OPENING / ENDGAME)
    // taken care by checkNoMoreAvailableMoves

    // (3) repeated moves in MIDGAME/ENDGAME (because won't happen in OPENING)
    // taken care by checkRepeatedMoves

}

//========================================================//
// CheckAvailableNeighbors (MIDGAME)                      //
//========================================================//
vector<string> checkAvailableNeighbors(string symbol[21], string remove_piece){
    
    vector<string> available_neighbors;
    
    int remove_piece_index = stoi(remove_piece) - 1; // index in Neighbor, e.g. "01" in Neighbor[0]

    const string* removePiece_neighbors = Neighbor[remove_piece_index];
    const int removePiece_neighborcount = NeighborCount[remove_piece_index];

    // for each neighbor of the remove piece
    for (int i=0; i<removePiece_neighborcount; i++){

        // loop through all symbol[], check whether the location is occupied
        for(int j=0;j<21;j++){

            // if the neighbor location is free
            if(removePiece_neighbors[i] == symbol[j]){ // neighbor[i] is the length of neighborcount, symbol[j] is the 21 location symbol
                available_neighbors.push_back(removePiece_neighbors[i]);
            }
        }
    }

    return available_neighbors;
}

//========================================================//
// Process generatePlacingPiece (OPENING)                 //
//========================================================//

struct placePiece generatePlacingPiece(string symbol[21]){
    string value;
    int i;
    struct placePiece place_info;

    bool inputInvalid = false;


    // input again point
    inputAgain:
    system("clear");
    displayGameBoard(symbol);

    // count to keep track of BW's turn

    // [[YOUR TURN]]
    if (player_turn == YOUR_TURN){

        //if (your_game_phrase == OPENING){ // MUST BE OPENING WHEN ENTERING HERE
            if (player_color == YOU_WHITE_COLOR){
                printf("\n\n\033[1m[[### YOUR TURN ###]]\033[0m");
                                
                // error message
                if (inputInvalid){
                    cout << "\n\033[1;31mThe input is NOT VALID, try again\033[0m" << endl;
                    inputInvalid = false;
                }
                
                printf("\nEnter Your Choice To ||PLACE|| \033[1;97;100m[White] (W)\033[0m: ");
            }
            else if (player_color == YOU_BLACK_COLOR){
                printf("\n\n\033[1m[[### YOUR TURN ###]]\033[0m");
                                
                // error message
                if (inputInvalid){
                    cout << "\n\033[1;31mThe input is NOT VALID, try again\033[0m" << endl;
                    inputInvalid = false;
                }
                
                printf("\nEnter Your Choice To ||PLACE|| \033[1;30;47m[Black] (B)\033[0m: ");
            }
            else printf("ERROR: player_color is not WHITE nor BLACK");
        //}
    }
    // [[OPPONENT'S TURN]]
    else if (player_turn == OPPONENT_TURN){

        //if (your_game_phrase == OPENING){
            if (player_color == YOU_BLACK_COLOR){
                printf("\n\n\033[1m[[### OPPONENT'S TURN ###]]\033[0m");
                
                // error message
                if (inputInvalid){
                    cout << "\n\033[1;31mThe input is NOT VALID, try again\033[0m" << endl;
                    inputInvalid = false;
                }
                
                printf("\nEnter Opponent Choice To ||PLACE|| \033[1;97;100m[White] (W)\033[0m: ");
            }
            else if (player_color == YOU_WHITE_COLOR){
                printf("\n\n\033[1m[[### OPPONENT'S TURN ###]]\033[0m");
                                
                // error message
                if (inputInvalid){
                    cout << "\n\033[1;31mThe input is NOT VALID, try again\033[0m" << endl;
                    inputInvalid = false;
                }
                
                printf("\nEnter Opponent Choice To ||PLACE|| \033[1;30;47m[Black] (B)\033[0m: ");
            }
            else printf("ERROR: player_color is not WHITE nor BLACK");
        //}
    }
    else printf("ERROR: player_turn is not YOUR TURN nor OPPONENT's TURN");



    // read the input
    cin >> value;
    value = inputValueHandler(value);
    

    // find the location, and set the placePiece place_info ch to the string we want to write on the board
    for(i=0;i<21;i++){

        // #############################################
        // FOUND THE LOCATION TO INSERT (OPENING)
        // #############################################
        if(value == symbol[i]){   // convert to string and compare
            place_info.pos = i;
            if(player_turn == YOUR_TURN && player_color == YOU_WHITE_COLOR)
                place_info.ch = " W";
            else if(player_turn == YOUR_TURN && player_color == YOU_BLACK_COLOR)
                place_info.ch = " B";
            else if(player_turn == OPPONENT_TURN && player_color == YOU_BLACK_COLOR)
                place_info.ch = " W";
            else if(player_turn == OPPONENT_TURN && player_color == YOU_WHITE_COLOR)
                place_info.ch = " B";

            break; // break for loop because it's found

        }else{
            place_info.pos = -1;
            place_info.ch = ' ';
        }
    }
    if(place_info.pos == -1){
        // set input invalid
        inputInvalid = true;
        goto inputAgain;
    }
    

    return place_info;
}


//========================================================//
// Process generateRemovingPiece                          //
//========================================================//

struct placePiece generateRemovingPiece(string symbol[21]){
    string remove_value;
    int i;
    struct placePiece remove_info;

    bool inputInvalid = false;
    bool cannotRemoveMillError = false;


    // input again point
    inputAgain:
    system("clear");
    displayGameBoard(symbol);

    

    // count to keep track of BW's turn

    // [[YOUR TURN]]
    if (player_turn == YOUR_TURN){

        //if (your_game_phrase == OPENING){ // MUST BE OPENING WHEN ENTERING HERE
            if (player_color == YOU_WHITE_COLOR){
                printf("\n\033[1;33mA Mill is Formed !! You can remove one of your opponent piece !!\033[0m");

                printf("\n\n\033[1m[[### YOUR TURN ###]]\033[0m");
                                
                // error message
                if (inputInvalid){
                    cout << "\n\033[1;31mThe input is NOT VALID, try again\033[0m" << endl;
                    inputInvalid = false;
                }
                if (cannotRemoveMillError){
                    cout << "\n\033[1;31mYou cannot remove a piece which is in a mill, try to remove another\033[0m" << endl;
                    cannotRemoveMillError = false;
                }
                
                printf("\nEnter Your Choice To ||REMOVE|| \033[1;30;47m[Black] (B)\033[0m: ");
            }
            else if (player_color == YOU_BLACK_COLOR){
                printf("\n\033[1;33mA Mill is Formed !! You can remove one of your opponent piece !!\033[0m");

                printf("\n\n\033[1m[[### YOUR TURN ###]]\033[0m");
                                
                // error message
                if (inputInvalid){
                    cout << "\n\033[1;31mThe input is NOT VALID, try again\033[0m" << endl;
                    inputInvalid = false;
                }
                if (cannotRemoveMillError){
                    cout << "\n\033[1;31mYou cannot remove a piece which is in a mill, try to remove another\033[0m" << endl;
                    cannotRemoveMillError = false;
                }
                
                printf("\nEnter Your Choice To ||REMOVE|| \033[1;97;100m[White] (W)\033[0m: ");
            }
            else printf("ERROR: player_color is not WHITE nor BLACK");
        //}
    }
    // [[OPPONENT'S TURN]]
    else if (player_turn == OPPONENT_TURN){

        //if (your_game_phrase == OPENING){
            if (player_color == YOU_BLACK_COLOR){
                printf("\n\033[1;33mA Mill is Formed !! Your opponent can remove one of your piece !!\033[0m");

                printf("\n\n\033[1m[[### OPPONENT'S TURN ###]]\033[0m");
                
                // error message
                if (inputInvalid){
                    cout << "\n\033[1;31mThe input is NOT VALID, try again\033[0m" << endl;
                    inputInvalid = false;
                }
                if (cannotRemoveMillError){
                    cout << "\n\033[1;31mYou cannot remove a piece which is in a mill, try to remove another\033[0m" << endl;
                    cannotRemoveMillError = false;
                }
                
                printf("\nEnter Opponent Choice To ||REMOVE|| \033[1;30;47m[Black] (B)\033[0m: ");
            }
            else if (player_color == YOU_WHITE_COLOR){
                printf("\n\033[1;33mA Mill is Formed !! Your opponent can remove one of your piece !!\033[0m");

                printf("\n\n\033[1m[[### OPPONENT'S TURN ###]]\033[0m");
                                
                // error message
                if (inputInvalid){
                    cout << "\n\033[1;31mThe input is NOT VALID, try again\033[0m" << endl;
                    inputInvalid = false;
                }
                if (cannotRemoveMillError){
                    cout << "\n\033[1;31mYou cannot remove a piece which is in a mill, try to remove another\033[0m" << endl;
                    cannotRemoveMillError = false;
                }
                
                printf("\nEnter Opponent Choice To ||REMOVE|| \033[1;97;100m[White] (W)\033[0m: ");
            }
            else printf("ERROR: player_color is not WHITE nor BLACK");
        //}
    }
    else printf("ERROR: player_turn is not YOUR TURN nor OPPONENT's TURN");



    // read the input
    cin >> remove_value;
    remove_value = inputValueHandler(remove_value);
    

    // check whether the remove value is valid
    // 1. check if valid digit
    bool isDigit = true;

    for (i=0; i<strlen(remove_value.c_str()); i++){
        if (!isdigit(remove_value.c_str()[i])){
            isDigit = false;
            break;
        }
    }

    // NOTE THAT!!!!!!
    // stoi(remove_value) is always needed to -1 because it is greater than the index by 1

    if (isDigit){
        if (stoi(remove_value)-1 >= 0 && stoi(remove_value)-1 < 21){
            // it is valid index
            // 2. check whether the correct player piece to remove  [IN THIS CASE, WE REMOVE THE OTHER'S PIECE, SO OPPOSITE COLOR]
            string remove_current_value;
            if(player_turn == YOUR_TURN && player_color == YOU_WHITE_COLOR)
                remove_current_value = " B";
            else if(player_turn == YOUR_TURN && player_color == YOU_BLACK_COLOR)
                remove_current_value = " W";
            else if(player_turn == OPPONENT_TURN && player_color == YOU_BLACK_COLOR)
                remove_current_value = " B";
            else if(player_turn == OPPONENT_TURN && player_color == YOU_WHITE_COLOR)
                remove_current_value = " W";

            if (remove_current_value == symbol[stoi(remove_value) - 1]){ // remove value e.g. 12, index = 11, so have to -1 !!!
                // 3. ONE MORE CHECK !!!!! CHECK WHETHER THE REMOVE_VALUE IS IN A MILL !!!
                // check formed a mill for the other
                bool formedAMill = checkFormMillForOther(symbol, remove_value);
                
                // 4. ONE MORE EXCEPTION CASE CHECK!!!
                // also check if all other pieces are milled
                bool allPiecesInMill = checkIfAllPiecesInMill(symbol);

                // not in a mill, nor all pieces in mill case, then remove!!!
                if (!formedAMill || allPiecesInMill) {
                    // valid input, remove_value itself is str already
                    remove_info.ch = remove_value;         // restore to the original number
                    remove_info.pos = stoi(remove_value) - 1;  // the pos is the same as the str // NOTE THAT INDEX IS ALWAYS 1 less than the text
                }
                else {
                    // remove piece not valid
                    remove_info.pos = -1;
                    remove_info.ch = ' ';
                    // instead of input invalid, show cannotRemoveMillError
                    cannotRemoveMillError = true;
                    goto inputAgain;
                }
            }
            else {
                // remove piece not valid
                remove_info.pos = -1;
                remove_info.ch = ' ';
            }
        }
        else {
            // remove piece not valid
            remove_info.pos = -1;
            remove_info.ch = ' ';
        }
    }
    else {
        // remove piece not valid
        remove_info.pos = -1;
        remove_info.ch = ' ';
    }

    if(remove_info.pos == -1){
        // set input invalid
        inputInvalid = true;
        goto inputAgain;
    }

    return remove_info;
}

//========================================================//
// Process generateMovingPieces (MIDGAME)                 //
//========================================================//
struct movePiece generateMovingPieces(string symbol[21]){
    string remove_value, moveto_value;
    int i;
    struct movePiece move_piece;
    struct placePiece removePiece_info;
    struct placePiece movetoPiece_info;

    bool inputInvalid = false;
    bool noAvailableNeighbors = false;

    // input again point
    inputAgain:
    system("clear");
    displayGameBoard(symbol);

    // count to keep track of BW's turn

    // [[YOUR TURN]]
    if (player_turn == YOUR_TURN){
        // MUST BE MIDGAME
        if (player_color == YOU_WHITE_COLOR){
            printf("\n\033[1m[[### YOUR TURN ###]]\033[0m");
                        
            // print error messages if applied
            if (inputInvalid){
                cout << "\n\033[1;31mThe input is NOT VALID, try again\033[0m" << endl;
                inputInvalid = false;
            }
            if (noAvailableNeighbors){
                cout << "\n\033[1;31mThere is no available neighbors for the piece you just chose, try another\033[0m" << endl;
                noAvailableNeighbors = false;
            }
            
            printf("\nEnter Your Choice To ||MOVE|| \033[1;97;100m[White] (W)\033[0m: ");
        }
        else if (player_color == YOU_BLACK_COLOR){
            printf("\n\033[1m[[### YOUR TURN ###]]\033[0m");
                        
            // print error messages if applied
            if (inputInvalid){
                cout << "\n\033[1;31mThe input is NOT VALID, try again\033[0m" << endl;
                inputInvalid = false;
            }
            if (noAvailableNeighbors){
                cout << "\n\033[1;31mThere is no available neighbors for the piece you just chose, try another\033[0m" << endl;
                noAvailableNeighbors = false;
            }
            
            printf("\nEnter Your Choice To ||MOVE|| \033[1;30;47m[Black] (B)\033[0m: ");
        }
        else printf("ERROR: player_color is not WHITE nor BLACK");
    }
    // [[OPPONENT'S TURN]]
    else if (player_turn == OPPONENT_TURN){
        // MUST BE MIDGAME
        if (player_color == YOU_BLACK_COLOR){
            printf("\n\033[1m[[### OPPONENT'S TURN ###]]\033[0m");

            // print error messages if applied
            if (inputInvalid){
                cout << "\n\033[1;31mThe input is NOT VALID, try again\033[0m" << endl;
                inputInvalid = false;
            }
            if (noAvailableNeighbors){
                cout << "\n\033[1;31mThere is no available neighbors for the piece you just chose, try another\033[0m" << endl;
                noAvailableNeighbors = false;
            }

            printf("\nEnter Opponent Choice To ||MOVE|| \033[1;97;100m[White] (W)\033[0m: ");
        }
        else if (player_color == YOU_WHITE_COLOR){
            printf("\n\033[1m[[### OPPONENT'S TURN ###]]\033[0m");
            
            // print error messages if applied
            if (inputInvalid){
                cout << "\n\033[1;31mThe input is NOT VALID, try again\033[0m" << endl;
                inputInvalid = false;
            }
            if (noAvailableNeighbors){
                cout << "\n\033[1;31mThere is no available neighbors for the piece you just chose, try another\033[0m" << endl;
                noAvailableNeighbors = false;
            }
            
            printf("\nEnter Opponent Choice To ||MOVE|| \033[1;30;47m[Black] (B)\033[0m: ");
        }
        else printf("ERROR: player_color is not WHITE nor BLACK");
    }
    else printf("ERROR: player_turn is not YOUR TURN nor OPPONENT's TURN");

   

    // read the input
    cin >> remove_value;
    remove_value = inputValueHandler(remove_value);


    // check whether the remove value is valid
    // 1. check if valid digit
    bool isDigit = true;

    for (i=0; i<strlen(remove_value.c_str()); i++){
        if (!isdigit(remove_value.c_str()[i])){
            isDigit = false;
            break;
        }
    }

    // NOTE THAT!!!!!!
    // stoi(remove_value) is always needed to -1 because it is greater than the index by 1

    if (isDigit){
        if (stoi(remove_value)-1 >= 0 && stoi(remove_value)-1 < 21){
            // it is valid index
            // 2. check whether the correct player piece to remove
            string remove_current_value;
            if(player_turn == YOUR_TURN && player_color == YOU_WHITE_COLOR)
                remove_current_value = " W";
            else if(player_turn == YOUR_TURN && player_color == YOU_BLACK_COLOR)
                remove_current_value = " B";
            else if(player_turn == OPPONENT_TURN && player_color == YOU_BLACK_COLOR)
                remove_current_value = " W";
            else if(player_turn == OPPONENT_TURN && player_color == YOU_WHITE_COLOR)
                remove_current_value = " B";

            if (remove_current_value == symbol[stoi(remove_value) - 1]){ // remove value e.g. 12, index = 11, so have to -1 !!!
                // valid input, remove_value itself is str already
                removePiece_info.ch = remove_value;         // restore to the original number
                removePiece_info.pos = stoi(remove_value) - 1;  // the pos is the same as the str // NOTE THAT INDEX IS ALWAYS 1 less than the text
            }
            else {
                // remove piece not valid
                removePiece_info.pos = -1;
                removePiece_info.ch = ' ';
            }
        }
        else {
            // remove piece not valid
            removePiece_info.pos = -1;
            removePiece_info.ch = ' ';
        }
    }
    else {
        // remove piece not valid
        removePiece_info.pos = -1;
        removePiece_info.ch = ' ';
    }

    if(removePiece_info.pos == -1){
        // set input invalid
        inputInvalid = true;
        goto inputAgain;
    }


    // passing this point, the removePiece is valid, then
    // check available neighbors for the input piece
    vector<string> available_neighbors;
    available_neighbors = checkAvailableNeighbors(symbol, remove_value);

    if (available_neighbors.size() == 0) {
        // set no available neighbors
        noAvailableNeighbors = true;
        goto inputAgain;
    }
    else {
        printf("The available position(s) for the piece %s move to are: ", remove_value.c_str());
        for (i=0; i<available_neighbors.size(); i++){
            printf(" %s",available_neighbors[i].c_str());
            if (i!=available_neighbors.size()-1) printf(",");
        }
    }

    // ask for input the moveto piece location
    // [[YOUR TURN]]
    if (player_turn == YOUR_TURN){
        // MUST BE MIDGAME
        if (player_color == YOU_WHITE_COLOR){
            printf("\n");
            printf("\nEnter Your Choice To ||MOVETO|| \033[1;97;100m[White] (W)\033[0m: ");
        }
        else if (player_color == YOU_BLACK_COLOR){
            printf("\n");
            printf("\nEnter Your Choice To ||MOVETO|| \033[1;30;47m[Black] (B)\033[0m: ");
        }
        else printf("ERROR: player_color is not WHITE nor BLACK");
    }
    // [[OPPONENT'S TURN]]
    else if (player_turn == OPPONENT_TURN){
        // MUST BE MIDGAME
        if (player_color == YOU_BLACK_COLOR){
            printf("\n");
            printf("\nEnter Opponent Choice To ||MOVETO|| \033[1;97;100m[White] (W)\033[0m: ");
        }
        else if (player_color == YOU_WHITE_COLOR){
            printf("\n");
            printf("\nEnter Opponent Choice To ||MOVETO|| \033[1;30;47m[Black] (B)\033[0m: ");
        }
        else printf("ERROR: player_color is not WHITE nor BLACK");
    }
    else printf("ERROR: player_turn is not YOUR TURN nor OPPONENT's TURN");


    // read the input
    cin >> moveto_value;
    moveto_value = inputValueHandler(moveto_value);


    // check whether match the available neighbors
    bool matchAvailableNeighbor = false;
    for (i=0; i<available_neighbors.size(); i++){
        if (moveto_value == available_neighbors[i]){
            matchAvailableNeighbor = true;
            break;
        }
    }
    if (!matchAvailableNeighbor){
        // set input invalid
        inputInvalid = true;
        goto inputAgain;
    }
    

    // find the location, and set the placePiece place_info ch to the string we want to write on the board
    for(i=0;i<21;i++){

        // #############################################
        // FOUND THE LOCATION TO INSERT (OPENING)
        // #############################################
        if(moveto_value == symbol[i]){   // convert to string and compare
            movetoPiece_info.pos = i;
            if(player_turn == YOUR_TURN && player_color == YOU_WHITE_COLOR)
                movetoPiece_info.ch = " W";
            else if(player_turn == YOUR_TURN && player_color == YOU_BLACK_COLOR)
                movetoPiece_info.ch = " B";
            else if(player_turn == OPPONENT_TURN && player_color == YOU_BLACK_COLOR)
                movetoPiece_info.ch = " W";
            else if(player_turn == OPPONENT_TURN && player_color == YOU_WHITE_COLOR)
                movetoPiece_info.ch = " B";
            break;
        }else{
            movetoPiece_info.pos = -1;
            movetoPiece_info.ch = ' ';
        }
    }
    if(movetoPiece_info.pos == -1){
        // set input invalid
        inputInvalid = true;
        goto inputAgain;
    }
    
    move_piece.removePiece = removePiece_info;
    move_piece.movetoPiece = movetoPiece_info;

    return move_piece;
}

//========================================================//
// Process generateFlyingPieces (ENDGAME)                 //
//========================================================//
struct movePiece generateFlyingPieces(string symbol[21]){
    string remove_value, flyto_value;
    int i;
    struct movePiece fly_piece;
    struct placePiece removePiece_info;
    struct placePiece flytoPiece_info;

    bool inputInvalid = false;

    // input again point
    inputAgain:
    system("clear");
    displayGameBoard(symbol);

    // count to keep track of BW's turn

    // [[YOUR TURN]]
    if (player_turn == YOUR_TURN){
        // MUST BE MIDGAME
        if (player_color == YOU_WHITE_COLOR){
            printf("\n\033[1m[[### YOUR TURN ###]]\033[0m");
                        
            // print error messages if applied
            if (inputInvalid){
                cout << "\n\033[1;31mThe input is NOT VALID, try again\033[0m" << endl;
                inputInvalid = false;
            }
            
            printf("\nEnter Your Choice To ||FLY|| \033[1;97;100m[White] (W)\033[0m: ");
        }
        else if (player_color == YOU_BLACK_COLOR){
            printf("\n\033[1m[[### YOUR TURN ###]]\033[0m");
                        
            // print error messages if applied
            if (inputInvalid){
                cout << "\n\033[1;31mThe input is NOT VALID, try again\033[0m" << endl;
                inputInvalid = false;
            }
            
            printf("\nEnter Your Choice To ||FLY|| \033[1;30;47m[Black] (B)\033[0m: ");
        }
        else printf("ERROR: player_color is not WHITE nor BLACK");
    }
    // [[OPPONENT'S TURN]]
    else if (player_turn == OPPONENT_TURN){
        // MUST BE MIDGAME
        if (player_color == YOU_BLACK_COLOR){
            printf("\n\033[1m[[### OPPONENT'S TURN ###]]\033[0m");

            // print error messages if applied
            if (inputInvalid){
                cout << "\n\033[1;31mThe input is NOT VALID, try again\033[0m" << endl;
                inputInvalid = false;
            }

            printf("\nEnter Opponent Choice To ||FLY|| \033[1;97;100m[White] (W)\033[0m: ");
        }
        else if (player_color == YOU_WHITE_COLOR){
            printf("\n\033[1m[[### OPPONENT'S TURN ###]]\033[0m");
            
            // print error messages if applied
            if (inputInvalid){
                cout << "\n\033[1;31mThe input is NOT VALID, try again\033[0m" << endl;
                inputInvalid = false;
            }
            
            printf("\nEnter Opponent Choice To ||FLY|| \033[1;30;47m[Black] (B)\033[0m: ");
        }
        else printf("ERROR: player_color is not WHITE nor BLACK");
    }
    else printf("ERROR: player_turn is not YOUR TURN nor OPPONENT's TURN");

   

    // read the input
    cin >> remove_value;
    remove_value = inputValueHandler(remove_value);


    // check whether the remove value is valid
    // 1. check if valid digit
    bool isDigit = true;

    for (i=0; i<strlen(remove_value.c_str()); i++){
        if (!isdigit(remove_value.c_str()[i])){
            isDigit = false;
            break;
        }
    }

    // NOTE THAT!!!!!!
    // stoi(remove_value) is always needed to -1 because it is greater than the index by 1

    if (isDigit){
        if (stoi(remove_value)-1 >= 0 && stoi(remove_value)-1 < 21){
            // it is valid index
            // 2. check whether the correct player piece to remove
            string remove_current_value;
            if(player_turn == YOUR_TURN && player_color == YOU_WHITE_COLOR)
                remove_current_value = " W";
            else if(player_turn == YOUR_TURN && player_color == YOU_BLACK_COLOR)
                remove_current_value = " B";
            else if(player_turn == OPPONENT_TURN && player_color == YOU_BLACK_COLOR)
                remove_current_value = " W";
            else if(player_turn == OPPONENT_TURN && player_color == YOU_WHITE_COLOR)
                remove_current_value = " B";

            if (remove_current_value == symbol[stoi(remove_value) - 1]){ // remove value e.g. 12, index = 11, so have to -1 !!!
                // valid input, remove_value itself is str already
                removePiece_info.ch = remove_value;         // restore to the original number
                removePiece_info.pos = stoi(remove_value) - 1;  // the pos is the same as the str // NOTE THAT INDEX IS ALWAYS 1 less than the text
            }
            else {
                // remove piece not valid
                removePiece_info.pos = -1;
                removePiece_info.ch = ' ';
            }
        }
        else {
            // remove piece not valid
            removePiece_info.pos = -1;
            removePiece_info.ch = ' ';
        }
    }
    else {
        // remove piece not valid
        removePiece_info.pos = -1;
        removePiece_info.ch = ' ';
    }

    if(removePiece_info.pos == -1){
        // set input invalid
        inputInvalid = true;
        goto inputAgain;
    }


    // passing this point, the removePiece is valid, then
    // no need to check available neighbors for the input piece
    

    // ask for input the flyto piece location
    // [[YOUR TURN]]
    if (player_turn == YOUR_TURN){
        // MUST BE MIDGAME
        if (player_color == YOU_WHITE_COLOR){
            printf("\nEnter Your Choice To ||FLYTO|| \033[1;97;100m[White] (W)\033[0m: ");
        }
        else if (player_color == YOU_BLACK_COLOR){
            printf("\nEnter Your Choice To ||FLYTO|| \033[1;30;47m[Black] (B)\033[0m: ");
        }
        else printf("ERROR: player_color is not WHITE nor BLACK");
    }
    // [[OPPONENT'S TURN]]
    else if (player_turn == OPPONENT_TURN){
        // MUST BE MIDGAME
        if (player_color == YOU_BLACK_COLOR){
            printf("\nEnter Opponent Choice To ||FLYTO|| \033[1;97;100m[White] (W)\033[0m: ");
        }
        else if (player_color == YOU_WHITE_COLOR){
            printf("\nEnter Opponent Choice To ||FLYTO|| \033[1;30;47m[Black] (B)\033[0m: ");
        }
        else printf("ERROR: player_color is not WHITE nor BLACK");
    }
    else printf("ERROR: player_turn is not YOUR TURN nor OPPONENT's TURN");


    // read the input
    cin >> flyto_value;
    flyto_value = inputValueHandler(flyto_value);


    // no need to check whether match the available neighbors
    

    // find the location, and set the placePiece place_info ch to the string we want to write on the board
    for(i=0;i<21;i++){

        // #############################################
        // FOUND THE LOCATION TO INSERT (OPENING)
        // #############################################
        if(flyto_value == symbol[i]){   // find whether the spot is free
            flytoPiece_info.pos = i;
            if(player_turn == YOUR_TURN && player_color == YOU_WHITE_COLOR)
                flytoPiece_info.ch = " W";
            else if(player_turn == YOUR_TURN && player_color == YOU_BLACK_COLOR)
                flytoPiece_info.ch = " B";
            else if(player_turn == OPPONENT_TURN && player_color == YOU_BLACK_COLOR)
                flytoPiece_info.ch = " W";
            else if(player_turn == OPPONENT_TURN && player_color == YOU_WHITE_COLOR)
                flytoPiece_info.ch = " B";
            break;
        }else{
            flytoPiece_info.pos = -1;
            flytoPiece_info.ch = ' ';
        }
    }
    if(flytoPiece_info.pos == -1){
        // set input invalid
        inputInvalid = true;
        goto inputAgain;
    }
    
    fly_piece.removePiece = removePiece_info;
    fly_piece.movetoPiece = flytoPiece_info;

    return fly_piece;
}


//========================================================//
// inputValueHandler (convert input variant)              //
//========================================================//
string inputValueHandler(string input){
    const char* c_input = input.c_str();
    // check whether the length is 1 and is digit, e.g. 1~9

    if (strlen(c_input)==1){
        if (isdigit(c_input[0])){
            return "0"+input; // e.g. 09
        }
        else return "INVALID";
    }

    // check whether the length is 2
    if (strlen(c_input)!=2){
        return "INVALID";
    }
    else {
        // it is fine originally
        if (isdigit(c_input[0])&&(isdigit(c_input[1]))){
            return input; // e.g. 15, no change
        }
        // if it is the location
        else if (isalpha(c_input[0])&&(isdigit(c_input[1]))){
            // to upper
            char c_0 = toupper(c_input[0]);
            char c_1 = c_input[1];

            // there are several ways to convert char to string, but cannot considered they are the same... will have errors
            string location; 
            location.push_back(c_0);
            location.push_back(c_1);

            return locToPos[location];
        }

    }

    return "INVALID"; // non-void function has to return something otherwise compiler will complain...
}

//============================================================//
// pieceCHtoWBXchar (convert the content ch in piece to W,B,x //
//============================================================//
char pieceCHtoWBXchar(string piece_ch){
    const char* c_piece_ch = piece_ch.c_str();

    char WBXchar;

    if (piece_ch == " W"){
        WBXchar = 'W';
    }
    else if (piece_ch == " B"){
        WBXchar = 'B';
    }
    else if (isdigit(c_piece_ch[0])&&(isdigit(c_piece_ch[1]))){
        WBXchar = 'x';
    }
    else {
        WBXchar ='?';
        printf("\n\nERROR in pieceCHtoWBXchar, the string piece_ch is %s, but failed to get char W/B/x ", c_piece_ch );
    }

    return WBXchar;
}

//================================================================//
// createOutputFile (use the time to name the outputfile)         //
//================================================================//
void createOutputFile(){
    // current date/time based on current system
    time_t current_time = time(0);  // Number of sec since January 1,1970, we have to convert it to readable time

    tm *local_tm = localtime(&current_time);
    int year = 1900 + local_tm->tm_year;    // tm_year is the year since 1900
    int month = 1 + local_tm->tm_mon;       // tm_mon is the index 0~11, need to +1
    int day = local_tm->tm_mday;            // tm_mday is from 1~31
    int hour = local_tm->tm_hour;           // tm_hour range is the 0~23
    int min = local_tm->tm_min;             // tm_min range is 0~59
    int sec = local_tm->tm_sec;             // tm_sec range is 0~59

    

    string filename;
    filename = to_string(year) + to_string(month) + to_string(day) + "_" 
                + inputValueHandler(to_string(hour)) + inputValueHandler(to_string(min)) + inputValueHandler(to_string(sec)) + "_gamerecord.txt";
    // add inputValueHandler to take care of the singledigit number, e.g. 00:18:07, we want 001807, not 0187

    ofs.open(filename, ofstream::app);  // app is the append mode, if the file is opened, continue to append, 
                                        // while ate is to erase and write from blank, but this doesn't matter in this program
                                        // since every record will be unique.
    
    ofs << "==================================================================== " << endl;
    ofs << "|| ############   NINE MEN'S MORRIS GAME VARIANT-B   ############ || " << endl;
    ofs << "==================================================================== " << endl;
    ofs << "Record time: " << ctime(&current_time) << endl; // ctime(&time(0)) can return the string form of the local time
    ofs << "==================================================================== " << endl;

}

//================================================================//
// getStateOfTheTurn (get the string representation of the state) //
//================================================================//
string getStateOfTheTurn (string symbol[21]){
    // current_state_string
    string current_state_string = "xxxxxxxxxxxxxxxxxxxxx";
    // string temp_char = " "; 
    // Note: inside a string are all chars, cannot replace with string, but char ok
    char temp_char = ' ';

    for (int i=0; i<21; i++) {
        if (symbol[i] == " W"){
            temp_char = 'W';
        }
        else if (symbol[i] == " B"){
            temp_char = 'B';
        }
        else {
            temp_char = 'x';
        }

        // update the current_state_string
        current_state_string[i] = temp_char;

    }

    cout << current_state_string << endl;


    return current_state_string;
}

//==============================================================//
// storeStateOfEachTurnInVecAndFile (keep track of the states ) //
//==============================================================//
void storeStateOfEachTurnInVecAndFile(string current_state_string, Action current_action, 
                                        placePiece place_info, placePiece remove_info, movePiece move_info){
    
    states_count++;         // original was 0, the first time would be 1
    
    // VECTOR PART
    // store current state string in game states
    game_states.push_back(current_state_string);


    // OUTPUT FILE PART
    string player_turn_str;
    string player_color_str;
    if (player_turn == YOUR_TURN) {
        player_turn_str = "YOUR TURN";

        if (player_color == YOU_WHITE_COLOR) player_color_str = "White (W)";
        else if (player_color == YOU_BLACK_COLOR) player_color_str = "Black (B)";

    }
    else if (player_turn == OPPONENT_TURN) {
        player_turn_str = "OPPONENT'S TURN";

        if (player_color == YOU_BLACK_COLOR) player_color_str = "White (W)";        // opposite as you
        else if (player_color == YOU_WHITE_COLOR) player_color_str = "Black (B)";

    }
    
    ofs << "\n === STATE #" << states_count << " ===" << endl;
    ofs << "[[### PLAYER TURN: " << player_turn_str << " COLOR: " << player_color_str << " ###]]" << endl;


    
    if (current_action == ACTION_PLACE) {
        string wbx_char;
        int place_index = place_info.pos;
        string place_location = inputValueHandler(to_string(place_index+1)); // index + 1 is the location, and convert it to 2-digit string

        wbx_char = pieceCHtoWBXchar(place_info.ch);

        ofs << "Place piece " << wbx_char << " at location " << posTextToLocationInMap(place_location) << "(" << place_location << ")"
                                                                << " [index" << place_index << "]" << endl;
    }

    else if (current_action == ACTION_REMOVE) {
        string wbx_char;
        int remove_index = remove_info.pos;
        string remove_location = inputValueHandler(to_string(remove_index+1)); 

        wbx_char = pieceCHtoWBXchar(remove_info.ch);

        ofs << "Remove piece " << wbx_char << " at location " << posTextToLocationInMap(remove_location) << "(" << remove_location << ")"
                                                                << " [index" << remove_index << "]" << endl;

    }

    else if (current_action == ACTION_MOVE) {
        string wbx_char;
        int move_index = move_info.removePiece.pos;
        string move_location = inputValueHandler(to_string(move_index+1)); 

        wbx_char = pieceCHtoWBXchar(move_info.movetoPiece.ch); // since we move and moveto the same color, so we don't need two wbx_char
                                                                // but we want movetoPiece.ch, that's the color
                                                                // removePiece.ch is the number index that we removed and stored back

        int moveto_index = move_info.movetoPiece.pos;
        string moveto_location = inputValueHandler(to_string(moveto_index+1)); 

        ofs << "Move piece " << wbx_char << " from location " << posTextToLocationInMap(move_location) << "(" << move_location << ")"
                                                                << " [index" << move_index << "]"
                                            << " to location " << posTextToLocationInMap(moveto_location) << "(" << moveto_location << ")"
                                                                << " [index" << moveto_index << "]" << endl;

    }

    else if (current_action == ACTION_FLY) {
        string wbx_char;
        int fly_index = move_info.removePiece.pos;
        string fly_location = inputValueHandler(to_string(fly_index+1)); 

        wbx_char = pieceCHtoWBXchar(move_info.movetoPiece.ch); // since we fly and flyto the same color, so we don't need two wbx_char
                                                                // but we want movetoPiece.ch, that's the color
                                                                // removePiece.ch is the number index that we removed and stored back

        int flyto_index = move_info.movetoPiece.pos;
        string flyto_location = inputValueHandler(to_string(flyto_index+1)); 

        ofs << "Fly piece " << wbx_char << " from location " << posTextToLocationInMap(fly_location) << "(" << fly_location << ")"
                                                                << " [index" << fly_index << "]"
                                            << " to location " << posTextToLocationInMap(flyto_location) << "(" << flyto_location << ")"
                                                                << " [index" << flyto_index << "]" << endl;

    }

    else ofs << "\n ERROR: current_action is " << current_action << ", which was not handled by the 4 types actions \n" << endl;



    ofs << current_state_string << endl << endl;
    



}



//==============================================================//
// colorDecorator (to process the color of some defined string) //
//==============================================================//
string colorDecorator(string original_str){
    string colored_text;

    if (original_str == " W"){
        colored_text = "\033[1;97;100m W\033[0m";
    }
    else if (original_str == " B"){
        colored_text = "\033[1;30;47m B\033[0m";
    }
    else colored_text = original_str; // if not defined, then unchanged

    return colored_text;
}


//========================================================//
// Display Game Board                                     //
//========================================================//

void displayGameBoard(string symbol[21]){

    // make a temp symbol array to store the color display
    string symbol_color[21];
    for (int i=0; i<21; i++){
        symbol_color[i] = colorDecorator(symbol[i]);
    }



	system("clear"); // different console command may vary

    // cout << symbol[0] << endl;
    // printf("%s\n",symbol[0].c_str());     
    // ### Note that printf expect char*, not string, have to use .c_str() or .data() to extract, otherwise wrong result

	printf("\033[36;1m=====================================================================================\033[0m\n");
	printf("\033[36;1m\t\t         Nine Men's Morris Game Variant-B\033[0m");
    printf("\033[36;1m\n=====================================================================================\033[0m\n");

    printf("// GAMEBOARD INPUT FORMAT => '01' (or '1') / 'A0' (or 'a0') to represent the bottom-left corner //\n");

    // remaining pieces
    int num_your_remaining_pieces = 9 - num_your_removed_pieces; 
    int num_opponent_remaining_pieces = 9 - num_opponent_removed_pieces; 

    // str_gamephrase
    string your_str_gamephrase;
    string opponent_str_gamephrase;

    if (your_game_phrase == OPENING) your_str_gamephrase = "Opening";
    else if (your_game_phrase == MIDGAME) your_str_gamephrase = "Midgame";
    else if (your_game_phrase == ENDGAME) your_str_gamephrase = "Endgame";

    if (opponent_game_phrase == OPENING) opponent_str_gamephrase = "Opening";
    else if (opponent_game_phrase == MIDGAME) opponent_str_gamephrase = "Midgame";
    else if (opponent_game_phrase == ENDGAME) opponent_str_gamephrase = "Endgame";

    if (color_order_mode == YOU_WHITE_FIRST) {
        printf("\nPlayer1(YOU     )[White](W) || Free pieces: %d || Total remaining pieces: %d || Game phrase: %s", num_your_pieces, num_your_remaining_pieces, your_str_gamephrase.c_str());
        printf("\nPlayer2(OPPONENT)[Black](B) || Free pieces: %d || Total remaining pieces: %d || Game phrase: %s", num_opponent_pieces, num_opponent_remaining_pieces, opponent_str_gamephrase.c_str());
    }
    if (color_order_mode == YOU_BLACK_FIRST) {
        printf("\nPlayer1(YOU     )[Black](B) || Free pieces: %d || Total remaining pieces: %d || Game phrase: %s", num_your_pieces, num_your_remaining_pieces, your_str_gamephrase.c_str());
        printf("\nPlayer2(OPPONENT)[White](W) || Free pieces: %d || Total remaining pieces: %d || Game phrase: %s", num_opponent_pieces, num_opponent_remaining_pieces, opponent_str_gamephrase.c_str());
    }
    if (color_order_mode == OPPONENT_WHITE_FIRST) {
        printf("\nPlayer1(OPPONENT)[White](W) || Free pieces: %d || Total remaining pieces: %d || Game phrase: %s", num_opponent_pieces, num_opponent_remaining_pieces, opponent_str_gamephrase.c_str());
        printf("\nPlayer2(YOU     )[Black](B) || Free pieces: %d || Total remaining pieces: %d || Game phrase: %s", num_your_pieces, num_your_remaining_pieces, your_str_gamephrase.c_str());
    }
    if (color_order_mode == OPPONENT_BLACK_FIRST) {
        printf("\nPlayer1(OPPONENT)[Black](B) || Free pieces: %d || Total remaining pieces: %d || Game phrase: %s", num_opponent_pieces, num_opponent_remaining_pieces, opponent_str_gamephrase.c_str());
        printf("\nPlayer2(YOU     )[White](W) || Free pieces: %d || Total remaining pieces: %d || Game phrase: %s", num_your_pieces, num_your_remaining_pieces, your_str_gamephrase.c_str());
    }
    
	printf("\n\t\t\t");
	printf("\n\t      \t  6     %s-------------%s-------------%s",symbol_color[18].c_str(),symbol_color[19].c_str(),symbol_color[20].c_str());
	printf("\n\t      \t         |              |              |");
	printf("\n\t      \t  5      |   %s--------%s--------%s    |",symbol_color[15].c_str(),symbol_color[16].c_str(),symbol_color[17].c_str());
	printf("\n\t      \t         |    |         |         |    |");
	printf("\n\t      \t  4      |    |   %s---%s---%s    |    |",symbol_color[12].c_str(),symbol_color[13].c_str(),symbol_color[14].c_str());
	printf("\n\t      \t         |    |    |         |    |    |");
   	printf("\n\t      \t  3     %s---%s---%s        %s---%s---%s",symbol_color[6].c_str(),symbol_color[7].c_str(),symbol_color[8].c_str(),symbol_color[9].c_str(),symbol_color[10].c_str(),symbol_color[11].c_str());
	printf("\n\t      \t         |    |    |         |    |    |");
	printf("\n\t      \t  2      |    |   %s--------%s    |    |",symbol_color[4].c_str(),symbol_color[5].c_str());
	printf("\n\t      \t         |    | /                 |    |");
	printf("\n\t      \t  1      |   %s------------------%s    |",symbol_color[2].c_str(),symbol_color[3].c_str());
    printf("\n\t      \t         |  /                          |");
	printf("\n\t      \t  0     %s----------------------------%s",symbol_color[0].c_str(),symbol_color[1].c_str());
    printf("\n\n");
    printf("\n\t\t \t A    B    C    D    E    F    G \n");

}

//===================================================================================================================================================//
// ##################################################################################################################################################//
// ############################################################ MENU FUNCTIONS ######################################################################//
// ##################################################################################################################################################//
//===================================================================================================================================================//

//========================================================//
// Menu functions                                         //
//========================================================//
void menu() {
    // menus

    while (program_mode == PROGRAM_MODE_NOT_SET){
        programModeMenu();
    }

    // if want to play morris game, go to further setting
    if (program_mode == PLAY_MORRIS_GAME) {

        while (game_mode == GAME_MODE_NOT_SET){
            gameModeMenu();
        }
        // if (game_mode != TWO_PLAYERS_MANUAL){
        //     while (ai_mode == AI_MODE_NOT_SET){
        //         aiModeMenu();
        //     }
        // }
        while (color_order_mode == COLOR_ORDER_NOT_SET){
            colorOrderModeMenu();
        }

        // set turn and color
        if (color_order_mode == YOU_WHITE_FIRST){
            player_turn = YOUR_TURN;
            player_color = YOU_WHITE_COLOR;
        }
        else if (color_order_mode == YOU_BLACK_FIRST){
            player_turn = YOUR_TURN;
            player_color = YOU_BLACK_COLOR;
        }
        else if (color_order_mode == OPPONENT_WHITE_FIRST){
            player_turn = OPPONENT_TURN;
            player_color = YOU_BLACK_COLOR;
        }
        else if (color_order_mode == OPPONENT_BLACK_FIRST){
            player_turn = OPPONENT_TURN;
            player_color = YOU_WHITE_COLOR;
        }
    }


}


void programModeMenu() {
    string str_choice;
    int choice = -1;
    bool inputInvalid = false;

    displayMenu:
    system("clear");

    cout << "\033[36;1m====================================================================\033[0m " << endl;
    cout << "\033[36;1m|| ############   NINE MEN'S MORRIS GAME VARIANT-B   ############ || \033[0m" << endl;
    cout << "\033[36;1m====================================================================\033[0m " << endl;
    cout << ">> Please choose the Program Mode: " << endl;
    cout << "\033[1m### Play Game: ###\033[0m" << endl;
    cout << "[0]: Play the Morris Game Variant-B" << endl;
    cout << "                         " << endl;
    cout << "\033[1m### Project Part I MINIMAX: ###\033[0m" << endl;
    cout << "[1]: MiniMaxOpening" << endl;
    cout << "[2]: MiniMaxGame" << endl;
    cout << "                         " << endl;
    cout << "\033[1m### Project Part II ALPHA-BETA Pruning: ###\033[0m" << endl;
    cout << "[3]: ABOpening" << endl;
    cout << "[4]: ABGame" << endl;
    cout << "                         " << endl;
    cout << "\033[1m### Project Part III PLAY A GAME FOR BLACK: ###\033[0m" << endl;
    cout << "[5]: MiniMaxOpeningBlack" << endl;
    cout << "[6]: MiniMaxGameBlack" << endl;
    cout << "                         " << endl;
    cout << "\033[1m### Project Part IV STATIC ESTIMATION: ###\033[0m" << endl;
    cout << "[7]: MiniMaxOpeningImproved" << endl;
    cout << "[8]: MiniMaxGameImproved" << endl;



    cout << endl << "Choice: " << endl;

    if (inputInvalid){
        // error message
        cout << "\n\nThe input is NOT VALID, try again\n" << endl;
    }
    inputInvalid = false;

    cin >> str_choice;

    choice = stoi(str_choice); //stoi in <string>

    switch(choice) {
    case 0:
        program_mode = PLAY_MORRIS_GAME;
        break;

    case 1:
        program_mode = MINIMAXOPENING;
        break;

    case 2:
        program_mode = MINIMAXGAME;
        break;

    case 3:
        program_mode = ABOPENING;
        break;

    case 4:
        program_mode = ABGAME;
        break;

    case 5:
        program_mode = MINIMAXOPENINGBLACK;
        break;

    case 6:
        program_mode = MINIMAXGAMEBLACK;
        break;

    case 7:
        program_mode = MINIMAXOPENINGIMPROVED;
        break;

    case 8:
        program_mode = MINIMAXGAMEIMPROVED;
        break;



    default:
        // set input invalid
        inputInvalid = true;

        // continue to display menu
        goto displayMenu;
        break;
    }

}


void gameModeMenu() {
    string str_choice;
    int choice = -1;
    bool inputInvalid = false;

    displayMenu:
    system("clear");

    cout << "\033[36;1m====================================================================\033[0m " << endl;
    cout << "\033[36;1m|| ############   NINE MEN'S MORRIS GAME VARIANT-B   ############ || \033[0m" << endl;
    cout << "\033[36;1m====================================================================\033[0m " << endl;
    cout << ">> Please choose the Game Mode: " << endl;
    cout << "[0]: 2 Players Manual Mode" << endl;
    cout << "[1]: AI helper Mode" << endl;
    cout << "[2]: AI opponent Mode" << endl;

    cout << endl << "Choice: " << endl;

    if (inputInvalid){
        // error message
        cout << "\n\nThe input is NOT VALID, try again\n" << endl;
    }
    inputInvalid = false;

    cin >> str_choice;

    choice = stoi(str_choice); //stoi in <string>

    switch(choice) {
    case 0:
        game_mode = TWO_PLAYERS_MANUAL;
        break;

    case 1:
        game_mode = AI_HELPER_MODE;
        break;

    case 2:
        game_mode = AI_OPPONENT_MODE;
        break;

    default:
        // set input invalid
        inputInvalid = true;

        // continue to display menu
        goto displayMenu;
        break;
    }

}

// void aiModeMenu() {
//     string str_choice;
//     int choice = -1;
//     bool inputInvalid = false;

//     displayMenu:
//     system("clear");

//     cout << "\033[36;1m====================================================================\033[0m " << endl;
//     cout << "\033[36;1m|| ############   NINE MEN'S MORRIS GAME VARIANT-B   ############ || \033[0m" << endl;
//     cout << "\033[36;1m====================================================================\033[0m " << endl;
//     cout << ">> Please choose the AI Mode: " << endl;
//     cout << "[0] AI HINT: shows the hint result" << endl;
//     cout << "[1] AI AUTO: makes the move automatically" << endl;

//     cout << endl << "Choice: " << endl;

//     if (inputInvalid){
//         // error message
//         cout << "\n\nThe input is NOT VALID, try again\n" << endl;
//     }
//     inputInvalid = false;

//     cin >> str_choice;

//     choice = stoi(str_choice); //stoi in <string>

//     switch(choice) {
//     case 0:
//         ai_mode = AI_HINT;
//         break;

//     case 1:
//         ai_mode = AI_AUTO;
//         break;

//     default:
//         // set input invalid
//         inputInvalid = true;

//         // continue to display menu
//         goto displayMenu;
//         break;
//     }

// }

void colorOrderModeMenu(){
    string str_choice;
    int choice = -1;
    bool inputInvalid = false;

    
    displayMenu:
    system("clear");
    
    cout << "\033[36;1m====================================================================\033[0m " << endl;
    cout << "\033[36;1m|| ############   NINE MEN'S MORRIS GAME VARIANT-B   ############ || \033[0m" << endl;
    cout << "\033[36;1m====================================================================\033[0m " << endl;
    cout << ">> Please choose your piece color and the order: " << endl;
    cout << "[0] You choose WHITE, and You move FIRST (default)" << endl;
    cout << "[1] You choose BLACK, and You move FIRST" << endl;
    cout << "[2] Opponent choose WHITE, and Opponent move FIRST" << endl;
    cout << "[3] Opponent choose BLACK, and Opponent move FIRST" << endl;

    cout << endl << "Choice: " << endl;

    if (inputInvalid){
        // error message
        cout << "\n\nThe input is NOT VALID, try again\n" << endl;
    }
    inputInvalid = false;

    cin >> str_choice;

    choice = stoi(str_choice); //stoi in <string>

    switch(choice) {
    case 0:
        color_order_mode = YOU_WHITE_FIRST;
        break;

    case 1:
        color_order_mode = YOU_BLACK_FIRST;
        break;

    case 2:
        color_order_mode = OPPONENT_WHITE_FIRST;
        break;

    case 3:
        color_order_mode = OPPONENT_BLACK_FIRST;
        break;

    default:
        // set input invalid
        inputInvalid = true;

        // continue to display menu
        goto displayMenu;
        break;
    }
}

void test() {
    vector<vector<string>> m01 = Mill_In_Game[0];
    vector<string> m01_1 = Mill_In_Game[0][0];

    for (int i=0; i<Mill_In_Game[0].size();i++){
        for (int j=0; j<Mill_In_Game[0][i].size();j++){
            cout << Mill_In_Game[0][i][j] << " ";
        }
        cout << endl;
    }

    // RESULT:
    // 01 07 19 
    // 01 03 05

    string test;
    cin >> test;
}


//===================================================================================================================================================//
// ##################################################################################################################################################//
// ################################################### PROJECT REQUIREMENT RELATED FUNCTIONS ########################################################//
// ##################################################################################################################################################//
//===================================================================================================================================================//

void MiniMaxOpening(){
    // reset game winning variables
    game_winner = WINNER_NOT_SET;
    game_winning_situation = WINNING_SITUATION_NOT_SET;

    cout << "IN MiniMaxOpening" << endl;
}
void MiniMaxGame(){
    // reset game winning variables
    game_winner = WINNER_NOT_SET;
    game_winning_situation = WINNING_SITUATION_NOT_SET;

    cout << "IN MiniMaxGame" << endl;
}

void ABOpening(){
    // reset game winning variables
    game_winner = WINNER_NOT_SET;
    game_winning_situation = WINNING_SITUATION_NOT_SET;
    cout << "IN ABOpening" << endl;
}
void ABGame(){
    // reset game winning variables
    game_winner = WINNER_NOT_SET;
    game_winning_situation = WINNING_SITUATION_NOT_SET;
    cout << "IN ABGame" << endl;
}

void MiniMaxOpeningBlack(){
    // reset game winning variables
    game_winner = WINNER_NOT_SET;
    game_winning_situation = WINNING_SITUATION_NOT_SET;
    cout << "IN MiniMaxOpeningBlack" << endl;
}
void MiniMaxGameBlack(){
    // reset game winning variables
    game_winner = WINNER_NOT_SET;
    game_winning_situation = WINNING_SITUATION_NOT_SET;
    cout << "IN MiniMaxGameBlack" << endl;
}
void MiniMaxOpeningImproved(){
    // reset game winning variables
    game_winner = WINNER_NOT_SET;
    game_winning_situation = WINNING_SITUATION_NOT_SET;
    cout << "IN MiniMaxOpeningImproved" << endl;
}
void MiniMaxGameImproved(){
    // reset game winning variables
    game_winner = WINNER_NOT_SET;
    game_winning_situation = WINNING_SITUATION_NOT_SET;
    cout << "IN MiniMaxGameImproved" << endl;
}