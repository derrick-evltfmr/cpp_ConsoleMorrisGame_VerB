#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
using namespace std;

#include "utility.h"

struct inputData {
    string ch;
    int i;
};

void menu();
void gameModeMenu();
void aiModeMenu();
void colorOrderModeMenu();
void run();
int check(string symbol[21],string ch,int count);
struct inputData inputValue(string symbol[21],int count);
void displayGameBoard(string symbol[21]);

//========================================================//
// Global variables                                       //
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

int main(){
    
    // start or restart
    string restartGame;
    playAgain:

    // show menu
    menu();

    // run game
	run();

    // after finish running the game, ask player whether to continue
	printf("\n\nIf You Want To Play Again Press 'Y', Else press 'Any'\n\n(Press Your Choice ...)");
	cin >> restartGame;
	if(restartGame == "Y" || restartGame == "y"){
        system("clear");
        goto playAgain;
    }
    else exit(0);
}

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
        cout << "\n\n The input is not valid\n\n" << endl;
    }

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
        cout << "\n\n The input is not valid\n\n" << endl;
    }

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
        cout << "\n\n The input is not valid\n\n" << endl;
    }

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

void run(){
    // initialize count, info, symbol arr
    int count = 0;
    struct inputData info;
    string symbol[21] = {"01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21"};
    
    // display the gameboard
    displayGameBoard(symbol);

    // continueLoop point, ask for input
    continueLoop:
    info = inputValue(symbol,count);
    symbol[info.i] = info.ch;

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

    system("clear");
    
    // display the gameboard
    displayGameBoard(symbol);
    if(check(symbol,info.ch,count)==1){
        // do nothing, not go back to continueLoop
    }
    else{
        count++;
        goto continueLoop;
    }
}

int check(string symbol[21],string ch,int count){
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

struct inputData inputValue(string symbol[21],int count){
    string value;
    int i;
    struct inputData info;

    bool inputInvalid = false;


    // input again point
    inputAgain:

    // count to keep track of BW's turn

    // [[YOUR TURN]]
    if (player_turn == YOUR_TURN){

        // OPENING
        if (your_game_phrase == OPENING){
            if (player_color == YOU_WHITE_COLOR){
                printf("\n\n[[### YOUR TURN ###]]");
                printf("\n\nEnter Your Choice [White] (W):");
            }
            else if (player_color == YOU_BLACK_COLOR){
                printf("\n\n[[### YOUR TURN ###]]");
                printf("\n\nEnter Your Choice [Black] (B):");
            }
            else printf("ERROR: player_color is not WHITE nor BLACK");
        }
    }
    // [[OPPONENT'S TURN]]
    else if (player_turn == OPPONENT_TURN){

        // OPENING
        if (your_game_phrase == OPENING){
            if (player_color == YOU_BLACK_COLOR){
                printf("\n\n[[### OPPONENT'S TURN ###]]");
                printf("\n\nEnter Opponent Choice [White] (W):");
            }
            else if (player_color == YOU_WHITE_COLOR){
                printf("\n\n[[### OPPONENT'S TURN ###]]");
                printf("\n\nEnter Opponent Choice [Black] (B):");
            }
            else printf("ERROR: player_color is not WHITE nor BLACK");
        }
    }
    else printf("ERROR: player_turn is not YOUR TURN nor OPPONENT TURN");

    if (inputInvalid){
        // error message
        cout << "\n\n The input is not valid\n\n" << endl;
    }

    // read the input
    cin >> value;
    
    for(i=0;i<21;i++){

        if(value == symbol[i]){   // convert to string and compare
            info.i = i;
            if(player_turn == YOUR_TURN && player_color == YOU_WHITE_COLOR)
                info.ch = " W";
            else if(player_turn == YOUR_TURN && player_color == YOU_BLACK_COLOR)
                info.ch = " B";
            else if(player_turn == OPPONENT_TURN && player_color == YOU_BLACK_COLOR)
                info.ch = " W";
            else if(player_turn == OPPONENT_TURN && player_color == YOU_WHITE_COLOR)
                info.ch = " B";
            break;
        }else{
            info.i = -1;
            info.ch = ' ';
        }
    }
    if(info.i == -1){
        // set input invalid
        inputInvalid = true;
        goto inputAgain;
    }
    

    return info;
}

void displayGameBoard(string symbol[21]){
	system("clear"); // different console command may vary

    // cout << symbol[0] << endl;
    // printf("%s\n",symbol[0].c_str());     
    // ### Note that printf expect char*, not string, have to use .c_str() or .data() to extract, otherwise wrong result

	printf("=====================================================================================\n");
	printf("\t\t         Nine Men's Morris Game Variant-B");
    printf("\n=====================================================================================\n");
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
	printf("\n\n\t\t\t");
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

