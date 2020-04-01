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
int check(string symbol[21],string ch);
string inputValueHandler(string input);
vector<string> checkAvailableNeighbors(string symbol[21], string remove_piece);
struct placePiece generatePlacingPiece(string symbol[21]);
struct placePiece generateRemovingPiece(string symbol[21]);
struct movePiece generateMovingPieces(string symbol[21]);
struct movePiece generateFlyingPieces(string symbol[21]);
void displayGameBoard(string symbol[21]);

void test();

//========================================================//
// Global Variables                                       //
//========================================================//
GameMode game_mode = GAME_MODE_NOT_SET;
AIMode ai_mode = AI_MODE_NOT_SET;
ColorOrderMode color_order_mode = COLOR_ORDER_NOT_SET;
PlayerTurn player_turn = TURN_NOT_DEFINED;
PlayerColor player_color = COLOR_NOT_DEFINED;

GamePhrase your_game_phrase = OPENING;
GamePhrase opponent_game_phrase = OPENING;

int num_your_pieces;
int num_opponent_pieces;
int num_your_removed_pieces;
int num_opponent_removed_pieces;

//========================================================//
// Extern Variables                                       //
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

    // after finish running the game, ask player whether to continue
	printf("\n\nIf you want to play again Press 'Y', otherwise Press 'Any other key'\n\n(Press Your Choice ...)");
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
    // initialize count, place_info, symbol arr
    // int count = 0;
    struct placePiece place_info;
    struct placePiece remove_info;
    struct movePiece move_info;

    string symbol[21] = {"01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21"}; // reset 
    
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
            }
            
        }

        //|| ============ ||
        //|| MIDGAME      ||
        //|| ============ ||
        if (your_game_phrase == MIDGAME) {
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
            }
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
            }
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
            }
        }

        
        //|| ============ ||
        //|| MIDGAME      ||
        //|| ============ ||
        if (opponent_game_phrase == MIDGAME) {
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
            }
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
            }
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
    // ### Check winning situation                    ###
    // ##################################################
    if(check(symbol,place_info.ch)==1){
        // do nothing, not go back to continueLoop
    }
    else{
        //count++;
        goto continueLoop;
    }
}

//========================================================//
// checkFormMill                                          //
//========================================================//
bool checkFormMill(string symbol[21], string place_piece){

    bool formedAMill = false;

    int place_piece_index = stoi(place_piece) - 1;

    vector<vector<string>> m01 = Mill_In_Game[0];
    vector<string> m01_1 = Mill_In_Game[0][0];

    int fillCount; // count whether 3 mill locations occupied

    // check through each mill set
    for (int i=0; i<Mill_In_Game[place_piece_index].size();i++){           //mill01 -> {mill01_1, mill01_2}
        
        fillCount = 0; // reset fillCount = 0
        
        // check through each mill element
        for (int j=0; j<Mill_In_Game[place_piece_index][i].size();j++){    //mill01_1 -> {"01", "07", "19"} / mill01_2 -> {"01", "03", "05"}
            // cout << Mill_In_Game[place_piece_index][i][j] << " ";
            
            // loop through all symbol[], check whether the location is occupied
            for(int k=0;k<21;k++){

                // if the neighbor location is free
                if(Mill_In_Game[place_piece_index][i][j] == symbol[k]){ // neighbor[i] is the length of neighborcount, symbol[j] is the 21 location symbol
                    fillCount++;
                }
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
// check game rules                                       //
//========================================================//

int check(string symbol[21],string ch){
    // int i;
    // for(i = 0;i<=6; i+=3)//it's for row
    //     if(symbol[i] == ch && symbol[i+1]==ch&&symbol[i+2]==ch){
    //         printf("\nthe Winner is : %c",ch);return 1;
    //     }
    // for(i = 0;i<3; i++)//it's for column
    //     if(symbol[i]==ch && symbol[i+3]==ch&&symbol[i+6]==ch){
    //         printf("\nthe Winner is : %c",ch);return 1;
    //     }
    // if(symbol[0]==ch && symbol[4]==ch&&symbol[8]==ch){
    //         printf("\nthe Winner is : %c",ch);return 1;
    //     }
    // else if(symbol[2]==ch && symbol[4]==ch && symbol[6]==ch){
    //         printf("\nthe Winner is : %c",ch);return 1;
    //     }
    // else if(count==8){
    //     printf("\nthe Game is DRAW");
    //     return 1;
    // }else return 0;

    return 0;
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
                // valid input, remove_value itself is str already
                remove_info.ch = remove_value;         // restore to the original number
                remove_info.pos = stoi(remove_value) - 1;  // the pos is the same as the str // NOTE THAT INDEX IS ALWAYS 1 less than the text
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
    cout << "Do nothing for now" << endl;

    struct movePiece move_piece;
    struct placePiece removePiece_info;
    struct placePiece movetoPiece_info;
    
    move_piece.removePiece = removePiece_info;
    move_piece.movetoPiece = movetoPiece_info;

    return move_piece;
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

    printf("[[ GAMEBOARD INPUT FORMAT: '01' or '1' or 'A0' to represent the bottom-left corner ]]\n");

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

    // initialize/reset num of pieces, game phrase
    num_your_pieces = 9;
    num_opponent_pieces = 9;
    num_your_removed_pieces = 0;
    num_opponent_removed_pieces = 0;

    your_game_phrase = OPENING;
    opponent_game_phrase = OPENING;
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