#include <stdio.h>
#include <stdlib.h>
#include <string>   // string, stoi()
#include <iostream>
using namespace std;

#include <vector>   // vector<T>
#include <cctype>   // isdigit()
#include <cstring>  // strlen()

#include "utility.h"

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
void menu();
void gameModeMenu();
void aiModeMenu();
void colorOrderModeMenu();
void run();

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
string getStateOfTheTurn(string symbol[21]);
void storeStateOfEachTurnInVecAndFile(string current_state_string);

void displayGameBoard(string symbol[21]);

void test();

//========================================================//
// Global Variables                                       //
//========================================================//
// handled by menus
GameMode game_mode = GAME_MODE_NOT_SET;
AIMode ai_mode = AI_MODE_NOT_SET;
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

//========================================================//
// Extern Variables (if constant and included then may not needed)//
//========================================================//
extern const string Neighbor[][4];  // defined in utility.h
extern const int NeighborCount[21]; // defined in utility.h
extern map<string,string> locToPos; // defined in utility.h

//========================================================//
// Main Program                                           //
//========================================================//
int main(){
    
    // // test code
    // test();

    // start or restart
    string restartGame;
    playAgain:

    // show menu
    menu();

    // run game
	run();

    // print game result
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
	printf("\n\nIf you want to play again Press 'Y', otherwise Press 'Any other key' to exit\n\n(Press Your Choice ...)");
	cin >> restartGame;
	if(restartGame == "Y" || restartGame == "y"){
        system("clear");
        goto playAgain;
    }
    else exit(0);
}

//========================================================//
// run the game                                           //
//========================================================//

void run(){
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


    // ====================================================================================================================================//




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
                // ##################################################
                // ### Store the state before go back or end      ###
                // ##################################################
                // at the end of the turn, the piece is set, before it goes back or ends, store the state
                string current_state_string = getStateOfTheTurn(symbol);
                storeStateOfEachTurnInVecAndFile(current_state_string);

                return;
            }
            // ##########################################################

            // check whether the input is valid[neighbor+empty] or not, if valid, generate two infos
            move_info = generateMovingPieces(symbol);
            // remove piece on board
            symbol[move_info.removePiece.pos] = move_info.removePiece.ch;
            // place piece on board
            symbol[move_info.movetoPiece.pos] = move_info.movetoPiece.ch;

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

            // if repeatedMoves and not formedAMill
            if (repeatedMoves && !formedAMill) {
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
                // ##################################################
                // ### Store the state before go back or end      ###
                // ##################################################
                // at the end of the turn, the piece is set, before it goes back or ends, store the state
                string current_state_string = getStateOfTheTurn(symbol);
                storeStateOfEachTurnInVecAndFile(current_state_string);

                return;
            }

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

            // if repeatedMoves and not formedAMill
            if (repeatedMoves && !formedAMill) {
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
                // ##################################################
                // ### Store the state before go back or end      ###
                // ##################################################
                // at the end of the turn, the piece is set, before it goes back or ends, store the state
                string current_state_string = getStateOfTheTurn(symbol);
                storeStateOfEachTurnInVecAndFile(current_state_string);

                return;
            }

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
                // ##################################################
                // ### Store the state before go back or end      ###
                // ##################################################
                // at the end of the turn, the piece is set, before it goes back or ends, store the state
                string current_state_string = getStateOfTheTurn(symbol);
                storeStateOfEachTurnInVecAndFile(current_state_string);

                return;
            }
            // ##########################################################


            // check whether the input is valid[neighbor+empty] or not, if valid, generate two infos
            move_info = generateMovingPieces(symbol);
            // remove piece on board
            symbol[move_info.removePiece.pos] = move_info.removePiece.ch;
            // place piece on board
            symbol[move_info.movetoPiece.pos] = move_info.movetoPiece.ch;

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

            // if repeatedMoves and not formedAMill
            if (repeatedMoves && !formedAMill) {
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
                // ##################################################
                // ### Store the state before go back or end      ###
                // ##################################################
                // at the end of the turn, the piece is set, before it goes back or ends, store the state
                string current_state_string = getStateOfTheTurn(symbol);
                storeStateOfEachTurnInVecAndFile(current_state_string);

                return;
            }

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

            // if repeatedMoves and not formedAMill
            if (repeatedMoves && !formedAMill) {
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
                // ##################################################
                // ### Store the state before go back or end      ###
                // ##################################################
                // at the end of the turn, the piece is set, before it goes back or ends, store the state
                string current_state_string = getStateOfTheTurn(symbol);
                storeStateOfEachTurnInVecAndFile(current_state_string);

                return;
            }

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
    // ### Store the state before go back or end      ###
    // ##################################################
    // at the end of the turn, the piece is set, before it goes back or ends, store the state
    string current_state_string = getStateOfTheTurn(symbol);
    storeStateOfEachTurnInVecAndFile(current_state_string);


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
    else printf("ERROR: in checkIfAllPieces, player_turn is not YOUR or OPPONENT turn.\n");

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
                printf("\n\n[[### YOUR TURN ###]]");
                                
                // error message
                if (inputInvalid){
                    cout << "\nThe input is NOT VALID, try again" << endl;
                    inputInvalid = false;
                }
                
                printf("\nEnter Your Choice To ||PLACE|| [White] (W): ");
            }
            else if (player_color == YOU_BLACK_COLOR){
                printf("\n\n[[### YOUR TURN ###]]");
                                
                // error message
                if (inputInvalid){
                    cout << "\nThe input is NOT VALID, try again" << endl;
                    inputInvalid = false;
                }
                
                printf("\nEnter Your Choice To ||PLACE|| [Black] (B): ");
            }
            else printf("ERROR: player_color is not WHITE nor BLACK");
        //}
    }
    // [[OPPONENT'S TURN]]
    else if (player_turn == OPPONENT_TURN){

        //if (your_game_phrase == OPENING){
            if (player_color == YOU_BLACK_COLOR){
                printf("\n\n[[### OPPONENT'S TURN ###]]");
                
                // error message
                if (inputInvalid){
                    cout << "\nThe input is NOT VALID, try again" << endl;
                    inputInvalid = false;
                }
                
                printf("\nEnter Opponent Choice To ||PLACE|| [White] (W): ");
            }
            else if (player_color == YOU_WHITE_COLOR){
                printf("\n\n[[### OPPONENT'S TURN ###]]");
                                
                // error message
                if (inputInvalid){
                    cout << "\nThe input is NOT VALID, try again" << endl;
                    inputInvalid = false;
                }
                
                printf("\nEnter Opponent Choice To ||PLACE|| [Black] (B): ");
            }
            else printf("ERROR: player_color is not WHITE nor BLACK");
        //}
    }
    else printf("ERROR: player_turn is not YOUR TURN nor OPPONENT TURN");



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
                printf("\nA Mill is Formed !! You can remove one of your opponent piece !!");

                printf("\n\n[[### YOUR TURN ###]]");
                                
                // error message
                if (inputInvalid){
                    cout << "\nThe input is NOT VALID, try again" << endl;
                    inputInvalid = false;
                }
                if (cannotRemoveMillError){
                    cout << "\nYou cannot remove a piece which is in a mill, try to remove another" << endl;
                    cannotRemoveMillError = false;
                }
                
                printf("\nEnter Your Choice To ||REMOVE|| [Black] (B): ");
            }
            else if (player_color == YOU_BLACK_COLOR){
                printf("\nA Mill is Formed !! You can remove one of your opponent piece !!");

                printf("\n\n[[### YOUR TURN ###]]");
                                
                // error message
                if (inputInvalid){
                    cout << "\nThe input is NOT VALID, try again" << endl;
                    inputInvalid = false;
                }
                if (cannotRemoveMillError){
                    cout << "\nYou cannot remove a piece which is in a mill, try to remove another" << endl;
                    cannotRemoveMillError = false;
                }
                
                printf("\nEnter Your Choice To ||REMOVE|| [White] (W): ");
            }
            else printf("ERROR: player_color is not WHITE nor BLACK");
        //}
    }
    // [[OPPONENT'S TURN]]
    else if (player_turn == OPPONENT_TURN){

        //if (your_game_phrase == OPENING){
            if (player_color == YOU_BLACK_COLOR){
                printf("\nA Mill is Formed !! Your opponent can remove one of your piece !!");

                printf("\n\n[[### OPPONENT'S TURN ###]]");
                
                // error message
                if (inputInvalid){
                    cout << "\nThe input is NOT VALID, try again" << endl;
                    inputInvalid = false;
                }
                if (cannotRemoveMillError){
                    cout << "\nYou cannot remove a piece which is in a mill, try to remove another" << endl;
                    cannotRemoveMillError = false;
                }
                
                printf("\nEnter Opponent Choice To ||REMOVE|| [Black] (B): ");
            }
            else if (player_color == YOU_WHITE_COLOR){
                printf("\nA Mill is Formed !! Your opponent can remove one of your piece !!");

                printf("\n\n[[### OPPONENT'S TURN ###]]");
                                
                // error message
                if (inputInvalid){
                    cout << "\nThe input is NOT VALID, try again" << endl;
                    inputInvalid = false;
                }
                if (cannotRemoveMillError){
                    cout << "\nYou cannot remove a piece which is in a mill, try to remove another" << endl;
                    cannotRemoveMillError = false;
                }
                
                printf("\nEnter Opponent Choice To ||REMOVE|| [White] (W): ");
            }
            else printf("ERROR: player_color is not WHITE nor BLACK");
        //}
    }
    else printf("ERROR: player_turn is not YOUR TURN nor OPPONENT TURN");



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
            printf("\n[[### YOUR TURN ###]]");
                        
            // print error messages if applied
            if (inputInvalid){
                cout << "\nThe input is NOT VALID, try again" << endl;
                inputInvalid = false;
            }
            if (noAvailableNeighbors){
                cout << "\nThere is no available neighbors for the piece you just chose, try another" << endl;
                noAvailableNeighbors = false;
            }
            
            printf("\nEnter Your Choice To ||MOVE|| [White] (W): ");
        }
        else if (player_color == YOU_BLACK_COLOR){
            printf("\n[[### YOUR TURN ###]]");
                        
            // print error messages if applied
            if (inputInvalid){
                cout << "\nThe input is NOT VALID, try again" << endl;
                inputInvalid = false;
            }
            if (noAvailableNeighbors){
                cout << "\nThere is no available neighbors for the piece you just chose, try another" << endl;
                noAvailableNeighbors = false;
            }
            
            printf("\nEnter Your Choice To ||MOVE|| [Black] (B): ");
        }
        else printf("ERROR: player_color is not WHITE nor BLACK");
    }
    // [[OPPONENT'S TURN]]
    else if (player_turn == OPPONENT_TURN){
        // MUST BE MIDGAME
        if (player_color == YOU_BLACK_COLOR){
            printf("\n[[### OPPONENT'S TURN ###]]");

            // print error messages if applied
            if (inputInvalid){
                cout << "\nThe input is NOT VALID, try again" << endl;
                inputInvalid = false;
            }
            if (noAvailableNeighbors){
                cout << "\nThere is no available neighbors for the piece you just chose, try another" << endl;
                noAvailableNeighbors = false;
            }

            printf("\nEnter Opponent Choice To ||MOVE|| [White] (W): ");
        }
        else if (player_color == YOU_WHITE_COLOR){
            printf("\n[[### OPPONENT'S TURN ###]]");
            
            // print error messages if applied
            if (inputInvalid){
                cout << "\nThe input is NOT VALID, try again" << endl;
                inputInvalid = false;
            }
            if (noAvailableNeighbors){
                cout << "\nThere is no available neighbors for the piece you just chose, try another" << endl;
                noAvailableNeighbors = false;
            }
            
            printf("\nEnter Opponent Choice To ||MOVE|| [Black] (B): ");
        }
        else printf("ERROR: player_color is not WHITE nor BLACK");
    }
    else printf("ERROR: player_turn is not YOUR TURN nor OPPONENT TURN");

   

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
            printf("\nEnter Your Choice To ||MOVETO|| [White] (W): ");
        }
        else if (player_color == YOU_BLACK_COLOR){
            printf("\n");
            printf("\nEnter Your Choice To ||MOVETO|| [Black] (B): ");
        }
        else printf("ERROR: player_color is not WHITE nor BLACK");
    }
    // [[OPPONENT'S TURN]]
    else if (player_turn == OPPONENT_TURN){
        // MUST BE MIDGAME
        if (player_color == YOU_BLACK_COLOR){
            printf("\n");
            printf("\nEnter Opponent Choice To ||MOVETO|| [White] (W): ");
        }
        else if (player_color == YOU_WHITE_COLOR){
            printf("\n");
            printf("\nEnter Opponent Choice To ||MOVETO|| [Black] (B): ");
        }
        else printf("ERROR: player_color is not WHITE nor BLACK");
    }
    else printf("ERROR: player_turn is not YOUR TURN nor OPPONENT TURN");


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
            printf("\n[[### YOUR TURN ###]]");
                        
            // print error messages if applied
            if (inputInvalid){
                cout << "\nThe input is NOT VALID, try again" << endl;
                inputInvalid = false;
            }
            
            printf("\nEnter Your Choice To ||FLY|| [White] (W): ");
        }
        else if (player_color == YOU_BLACK_COLOR){
            printf("\n[[### YOUR TURN ###]]");
                        
            // print error messages if applied
            if (inputInvalid){
                cout << "\nThe input is NOT VALID, try again" << endl;
                inputInvalid = false;
            }
            
            printf("\nEnter Your Choice To ||FLY|| [Black] (B): ");
        }
        else printf("ERROR: player_color is not WHITE nor BLACK");
    }
    // [[OPPONENT'S TURN]]
    else if (player_turn == OPPONENT_TURN){
        // MUST BE MIDGAME
        if (player_color == YOU_BLACK_COLOR){
            printf("\n[[### OPPONENT'S TURN ###]]");

            // print error messages if applied
            if (inputInvalid){
                cout << "\nThe input is NOT VALID, try again" << endl;
                inputInvalid = false;
            }

            printf("\nEnter Opponent Choice To ||FLY|| [White] (W): ");
        }
        else if (player_color == YOU_WHITE_COLOR){
            printf("\n[[### OPPONENT'S TURN ###]]");
            
            // print error messages if applied
            if (inputInvalid){
                cout << "\nThe input is NOT VALID, try again" << endl;
                inputInvalid = false;
            }
            
            printf("\nEnter Opponent Choice To ||FLY|| [Black] (B): ");
        }
        else printf("ERROR: player_color is not WHITE nor BLACK");
    }
    else printf("ERROR: player_turn is not YOUR TURN nor OPPONENT TURN");

   

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
            printf("\nEnter Your Choice To ||FLYTO|| [White] (W): ");
        }
        else if (player_color == YOU_BLACK_COLOR){
            printf("\nEnter Your Choice To ||FLYTO|| [Black] (B): ");
        }
        else printf("ERROR: player_color is not WHITE nor BLACK");
    }
    // [[OPPONENT'S TURN]]
    else if (player_turn == OPPONENT_TURN){
        // MUST BE MIDGAME
        if (player_color == YOU_BLACK_COLOR){
            printf("\nEnter Opponent Choice To ||FLYTO|| [White] (W): ");
        }
        else if (player_color == YOU_WHITE_COLOR){
            printf("\nEnter Opponent Choice To ||FLYTO|| [Black] (B): ");
        }
        else printf("ERROR: player_color is not WHITE nor BLACK");
    }
    else printf("ERROR: player_turn is not YOUR TURN nor OPPONENT TURN");


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
            toupper(c_input[0]);
            string location(c_input); // use constructor to convert c_input back to string location
            return locToPos[location];
        }

    }

    return "INVALID"; // non-void function has to return something otherwise compiler will complain...
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
        else if (symbol[i] == " W"){
            temp_char = 'B';
        }
        else {
            temp_char = 'x';
        }

        // update the current_state_string
        current_state_string[i] = temp_char;

    }

    cout << current_state_string << endl;

}

//==============================================================//
// storeStateOfEachTurnInVecAndFile (keep track of the states ) //
//==============================================================//
void storeStateOfEachTurnInVecAndFile(string current_state_string){


}







//========================================================//
// Display Game Board                                     //
//========================================================//

void displayGameBoard(string symbol[21]){
	system("clear"); // different console command may vary

    // cout << symbol[0] << endl;
    // printf("%s\n",symbol[0].c_str());     
    // ### Note that printf expect char*, not string, have to use .c_str() or .data() to extract, otherwise wrong result

	printf("=====================================================================================\n");
	printf("\t\t         Nine Men's Morris Game Variant-B");
    printf("\n=====================================================================================\n");

    printf("// GAMEBOARD INPUT FORMAT => '01' or '1' or 'A0' to represent the bottom-left corner //\n");

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
	printf("\n\t      \t  6     %s-------------%s-------------%s",symbol[18].c_str(),symbol[19].c_str(),symbol[20].c_str());
	printf("\n\t      \t         |              |              |");
	printf("\n\t      \t  5      |   %s--------%s--------%s    |",symbol[15].c_str(),symbol[16].c_str(),symbol[17].c_str());
	printf("\n\t      \t         |    |         |         |    |");
	printf("\n\t      \t  4      |    |   %s---%s---%s    |    |",symbol[12].c_str(),symbol[13].c_str(),symbol[14].c_str());
	printf("\n\t      \t         |    |    |         |    |    |");
   	printf("\n\t      \t  3     %s---%s---%s        %s---%s---%s",symbol[6].c_str(),symbol[7].c_str(),symbol[8].c_str(),symbol[9].c_str(),symbol[10].c_str(),symbol[11].c_str());
	printf("\n\t      \t         |    |    |         |    |    |");
	printf("\n\t      \t  2      |    |   %s--------%s    |    |",symbol[4].c_str(),symbol[5].c_str());
	printf("\n\t      \t         |    | /                 |    |");
	printf("\n\t      \t  1      |   %s------------------%s    |",symbol[2].c_str(),symbol[3].c_str());
    printf("\n\t      \t         |  /                          |");
	printf("\n\t      \t  0     %s----------------------------%s",symbol[0].c_str(),symbol[1].c_str());
    printf("\n\n");
    printf("\n\t\t \t A    B    C    D    E    F    G \n");

}

//========================================================//
// Menu functions                                         //
//========================================================//
void menu() {
    // menus
    while (game_mode == GAME_MODE_NOT_SET){
        gameModeMenu();
    }
    if (game_mode != TWO_PLAYERS_MANUAL){
        while (ai_mode == AI_MODE_NOT_SET){
            aiModeMenu();
        }
    }
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

void gameModeMenu() {
    string str_choice;
    int choice = -1;
    bool inputInvalid = false;

    displayMenu:
    system("clear");

    cout << "==================================================================== " << endl;
    cout << "|| ############   NINE MEN'S MORRIS GAME VARIANT-B   ############ || " << endl;
    cout << "==================================================================== " << endl;
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

void aiModeMenu() {
    string str_choice;
    int choice = -1;
    bool inputInvalid = false;

    displayMenu:
    system("clear");

    cout << "==================================================================== " << endl;
    cout << "|| ############   NINE MEN'S MORRIS GAME VARIANT-B   ############ || " << endl;
    cout << "==================================================================== " << endl;
    cout << ">> Please choose the AI Mode: " << endl;
    cout << "[0] AI HINT: shows the hint result" << endl;
    cout << "[1] AI AUTO: makes the move automatically" << endl;

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
        ai_mode = AI_HINT;
        break;

    case 1:
        ai_mode = AI_AUTO;
        break;

    default:
        // set input invalid
        inputInvalid = true;

        // continue to display menu
        goto displayMenu;
        break;
    }

}

void colorOrderModeMenu(){
    string str_choice;
    int choice = -1;
    bool inputInvalid = false;

    
    displayMenu:
    system("clear");
    
    cout << "==================================================================== " << endl;
    cout << "|| ############   NINE MEN'S MORRIS GAME VARIANT-B   ############ || " << endl;
    cout << "==================================================================== " << endl;
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