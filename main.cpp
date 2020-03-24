#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
using namespace std;

struct myDataType {
    char ch;
    int i;
};

void run();
int check(char symbol[21],char ch,int count);
struct myDataType inputValue(char symbol[21],int count);
void Display(string symbol[21]);

int main(){
    char reStart;
    again:
	run();
	printf("\n\nIf You Want To Play Again Press '1', Else press 'Any'\n\n(Press Your Choice ...)");
	scanf("%s",&reStart);
	if(reStart == '1')
        {
            system("clear");
            goto again;
        }
    else
        exit(0);
}

void run(){
    int count = 0;
    struct myDataType info;
    string symbol[21] = {"01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21"};
    Display(symbol);
    // again:
    // info = inputValue(symbol,count);
    // symbol[info.i] = info.ch;
    // system("clear");
    // Display(symbol);
    // if(check(symbol,info.ch,count)==1);
    // else{
    //     count++;
    //     goto again;
    // }
}
int check(char symbol[21],char ch,int count){   //**** check for 21positions later ****//
    int i;
    for(i = 0;i<=6; i+=3)//it's for row
        if(symbol[i] == ch && symbol[i+1]==ch&&symbol[i+2]==ch){
            printf("\nthe Winner is : %c",ch);return 1;
        }
    for(i = 0;i<3; i++)//it's for column
        if(symbol[i]==ch && symbol[i+3]==ch&&symbol[i+6]==ch){
            printf("\nthe Winner is : %c",ch);return 1;
        }
    if(symbol[0]==ch && symbol[4]==ch&&symbol[8]==ch){
            printf("\nthe Winner is : %c",ch);return 1;
        }
    else if(symbol[2]==ch && symbol[4]==ch && symbol[6]==ch){
            printf("\nthe Winner is : %c",ch);return 1;
        }
    else if(count==8){
        printf("\nthe Game is DRAW");
        return 1;
    }else return 0;
}
struct myDataType inputValue(char symbol[21],int count){    //**** check for 21positions later ****//
    char value;
    int i;
    struct myDataType info;
    inputAgain:
    if(count%2 == 0){
        printf("\nEnter Your Choice [White] (W):");
    }else{
        printf("\nEnter Your Choice [Black] (B):");
    }
    scanf("%s",&value);
    for(i=0;i<9;i++){

        if(value == symbol[i]){
            info.i = i;
            if(count%2 == 0)
                info.ch = 'W';
            else
                info.ch = 'B';
            break;
        }else{
            info.i = -1;
            info.ch = ' ';
        }
    }
    if(info.i == -1){
        printf("\nInput is not Valid");
        goto inputAgain;
    }
    return info;
}

void Display(string symbol[21]){
	system("clear"); // different console command may vary

    // cout << symbol[0] << endl;
    // printf("%s\n",symbol[0].c_str());     
    // ### Note that printf expect char*, not string, have to use .c_str() or .data() to extract, otherwise wrong result

	printf("=====================================================================================\n");
	printf("\t\t         Nine Men's Morris Game Variant-B");
    printf("\n=====================================================================================");
	printf("\nPlayers 1 symbol: [White](W)");
	printf("\nPlayers 2 symbol: [Black](B)");
	printf("\n\t\t\t");
	printf("\n\t\t\t%s-------------%s-------------%s",symbol[18].c_str(),symbol[19].c_str(),symbol[20].c_str());
	printf("\n\t\t\t |              |              |");
	printf("\n\t\t\t |   %s--------%s--------%s    |",symbol[15].c_str(),symbol[16].c_str(),symbol[17].c_str());
	printf("\n\t\t\t |    |         |         |    |");
	printf("\n\t\t\t |    |   %s---%s---%s    |    |",symbol[12].c_str(),symbol[13].c_str(),symbol[14].c_str());
	printf("\n\t\t\t |    |    |         |    |    |");
   	printf("\n\t\t\t%s---%s---%s        %s---%s---%s",symbol[6].c_str(),symbol[7].c_str(),symbol[8].c_str(),symbol[9].c_str(),symbol[10].c_str(),symbol[11].c_str());
	printf("\n\t\t\t |    |    |         |    |    |");
	printf("\n\t\t\t |    |   %s--------%s    |    |",symbol[4].c_str(),symbol[5].c_str());
	printf("\n\t\t\t |    | /                 |    |");
	printf("\n\t\t\t |    %s-----------------%s    |",symbol[2].c_str(),symbol[3].c_str());
    printf("\n\t\t\t |  /                          |");
	printf("\n\t\t\t%s----------------------------%s",symbol[0].c_str(),symbol[1].c_str());
}

