#include<stdio.h>
#include<stdlib.h>

struct myDataType {
    char ch;
    int i;
};

void run();
int check(char sym[21],char ch,int count);
struct myDataType inputValue(char sym[21],int count);
void Display(char sym[21]);

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
    char symbol[21] = {'01','02','03','04','05','06','07','08','09','10','11','12','13','14','15','16','17','18','19','20','21'};
    Display(symbol);
    again:
    info = inputValue(symbol,count);
    symbol[info.i] = info.ch;
    system("clear");
    Display(symbol);
    if(check(symbol,info.ch,count)==1);
    else{
        count++;
        goto again;
    }
}
int check(char sym[21],char ch,int count){   //**** check for 21positions later ****//
    int i;
    for(i = 0;i<=6; i+=3)//it's for row
        if(sym[i] == ch && sym[i+1]==ch&&sym[i+2]==ch){
            printf("\nthe Winner is : %c",ch);return 1;
        }
    for(i = 0;i<3; i++)//it's for column
        if(sym[i]==ch && sym[i+3]==ch&&sym[i+6]==ch){
            printf("\nthe Winner is : %c",ch);return 1;
        }
    if(sym[0]==ch && sym[4]==ch&&sym[8]==ch){
            printf("\nthe Winner is : %c",ch);return 1;
        }
    else if(sym[2]==ch && sym[4]==ch && sym[6]==ch){
            printf("\nthe Winner is : %c",ch);return 1;
        }
    else if(count==8){
        printf("\nthe Game is DRAW");
        return 1;
    }else return 0;
}
struct myDataType inputValue(char sym[21],int count){    //**** check for 21positions later ****//
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

        if(value == sym[i]){
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

void Display(char sym[21]){
	system("clear"); // different console command may vary
	printf("======================================================================\n");
	printf("\t\t\t      Nine Men's Morris Game Variant-B");
        printf("\n======================================================================");
	printf("\nPlayers 1 Symbol: [White](W)");
	printf("\nPlayers 2 Symbol: [Black](B)");
	printf("\n\t\t\t");
	printf("\n\t\t\t%c----|----|---%c----|---%c----|",sym[18],sym[19],sym[20]);
	printf("\n\t\t\t |              |              |");
	printf("\n\t\t\t |   %c----|---%c----|---%c    |",sym[15],sym[16],sym[17]);
	printf("\n\t\t\t |    |         |         |    |");
	printf("\n\t\t\t |    |   %c---%c---%c    |    |",sym[12],sym[13],sym[14]);
	printf("\n\t\t\t |    |    |         |    |    |");
   	printf("\n\t\t\t%c---%c---%c        %c---%c---%c",sym[6],sym[7],sym[8],sym[9],sym[10],sym[11]);
	printf("\n\t\t\t |    |    |         |    |    |");
	printf("\n\t\t\t |    |   %c----|---%c    |    |",sym[4],sym[5]);
	printf("\n\t\t\t |    | /                 |    |");
	printf("\n\t\t\t |    %c---|----|----|---%c----|",sym[2],sym[3]);
    printf("\n\t\t\t |  /                          |");
	printf("\n\t\t\t%c----|----|----|----|----|---%c",sym[0],sym[1]);
}

