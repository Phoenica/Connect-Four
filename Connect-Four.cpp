#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <time.h>
std::stack <std::pair<int,int> > last;

int buf;
void clear_board(char tab[7][6])
{
    for(int i=0; i<7; i++)for(int j=0; j<6; j++)tab[i][j] = '.';
}
void print_board(char tab[7][6])
{
    for(int i=1; i<8;i++)printf("%d",i); printf("\n");
    for(int i=0; i<6; i++){for(int j=0; j<7; j++)printf("%c",tab[j][i]); printf("\n");}
}
int counter(int buf)
{
    return buf/100+(buf/10-(buf/100)*10)+buf%10;
}
int recalculate_buf(char tab[7][6],int x, int y,int buf,char a, char foe)
{
    if(tab[x][y]==a) return buf - 10;
    if(tab[x][y]==foe) return buf - 100;
    else return buf-1;
}
int scoring(char tab[7][6],std::pair<int,int> dir,int i,int j,char a)
{
    char foe;
    int score = 0;
    if(a == 'X') foe = 'O'; else foe = 'X';
    int g;
    if(tab[j][i]=='.') {buf++; }
    if(tab[j][i] == a)
        {
            if(buf<100){ buf= buf + 10; }
            else
            {
                buf = 10;
                g = j-1;
                int temp = i-dir.second;
                while(tab[g][temp] != foe) {buf++; g--;temp = temp - dir.second;}
            }
        }
    if(tab[j][i] == foe)
    {
        if(buf<10 || buf>=100) {buf= buf + 100;}
        else
        {
            int temp = i-dir.second;
            buf = 100;
            g = j-1;
            while(tab[g][temp] != a) { buf++; g--; temp = temp - dir.second; }
        }
    }
    if(counter(buf)==4)
        {
            if(buf < 10) buf--; else
            if(buf < 20) { score = score + 1;  buf = recalculate_buf(tab,j-3*dir.first,i-3*dir.second,buf,a,foe);  } else
            if(buf < 30) {score = score + 3; buf = recalculate_buf(tab,j-3*dir.first,i-3*dir.second,buf,a,foe);} else
            if(buf < 40) {score = score + 7; buf = recalculate_buf(tab,j-3*dir.first,i-3*dir.second,buf,a,foe);}else
            if(buf < 200) {score = score - 2; buf =recalculate_buf(tab,j-3*dir.first,i-3*dir.second,buf,a,foe);} else
            if(buf < 300) {score = score - 4;  buf =recalculate_buf(tab,j-3*dir.first,i-3*dir.second,buf,a,foe);} else
            return -999999;
        }
    return score;
}
int score(char tab[7][6],char a)
{
    int i,j;
    int score=0;
    int tempscore = 0;
    std::pair<int,int> dir;
    dir.first = 1; dir.second = 0;
    //zliczanie -
    for(i = 0; i < 6; i++)
        for(j = 0,buf = 0; j < 7; j++)
        {
            tempscore = scoring(tab,dir,i,j,a);
            if(tempscore == -999999) return tempscore;
            score = score + tempscore;
        }
    //zliczanie |
     dir.first = 0; dir.second = 1;
    for(j = 0; j < 7; j++)
        for(i = 0,buf = 0; i < 6; i++)
        {
            tempscore = (int)scoring(tab,dir,i,j,a);
            if(tempscore == -999999) return tempscore;
            score = score + tempscore/2;
        }
    //zliczanie '/' i '\'
    dir.first = 1; dir.second = -1;
    for(int k = 3; k < 6; k++)
        for(j = 0,buf = 0,i = k; i >= 0 && j < 7; j++,i--)
        {
            tempscore = scoring(tab,dir,i,j,a);
            if(tempscore == -999999) return tempscore;
            score = score + tempscore;
        }
    for(int k = 1; k < 4; k++)
        for(i = 5,buf = 0,j = k; i >= 0 && j < 7; j++,i--)
        {
            tempscore = scoring(tab,dir,i,j,a);
            if(tempscore == -999999) return tempscore;
            score = score + tempscore;
        }

        dir.first = 1; dir.second = 1;
    for(int k = 1; k < 4; k++)
        for(i = 0,buf = 0,j = k; i < 6 && j < 7; j++,i++)
        {
            tempscore = scoring(tab,dir,i,j,a);
            if(tempscore == -999999) return tempscore;
            score = score + tempscore;
        }
    for(int k = 0; k < 3; k++)
        for(j = 0,buf = 0,i = k; i < 6 && j < 7; j++,i++)
        {
            tempscore = scoring(tab,dir,i,j,a);
            if(tempscore == -999999) return tempscore;
            score = score + tempscore;
        }
    return score;
}
int check(char tab[7][6],char a, int x, int y)
{
    int buf = 1;
    int i,j;
    //sprawdzamy -
    for(i = x+1; i < 7; i++ ) if(tab[i][y]== a) buf++; else break;
    for(i = x-1; i >= 0; i-- ) if(tab[i][y]== a) buf++; else break;
    if(buf >= 4)  return 999999;
    //sprawdzanie |
    buf = 1;
    for(i = y+1; i < 6; i++ ) if(tab[x][i]== a) buf++; else break;
    for(i = y-1; i >= 0; i-- ) if(tab[x][i]== a) buf++; else break;
    if(buf >= 4) return 999999;
    //sprawdzanie /
    buf = 1;
    for(i = x + 1,j = y + 1; i < 7 && j < 6; i++, j++ ) if(tab[i][j]== a) buf++; else break;
    for(i = x - 1,j = y - 1; i < 7 && j < 6; i--, j-- ) if(tab[i][j]== a) buf++; else break;
     if(buf >= 4) return 999999;
    //sprawdzanie \ /
    buf = 1;
    for(i = x - 1,j = y + 1; i >= 0 && j < 6; i--, j++ ) if(tab[i][j]== a) buf++; else break;
    for(i = x + 1,j = y - 1; i < 7 && j >= 0; i++, j-- ) if(tab[i][j]== a) buf++; else break;
     if(buf >= 4) return 999999;
    return 0;
}
int add(char tab[7][6],char a, int x)
{
    if(x > 6 || x < 0) {printf("Zla kolumna %d\n",x); return -1;}
    for(int i = 5; i >= 0;i--)
    {
        if(tab[x][i]=='.') {tab[x][i] = a; last.push(std::make_pair(x,i));   return check(tab,a,x,i); }
    }
    printf("Nie ma miejsca w kolumnie %d\n",x); return -1;
}
int add_AI(char tab[7][6],char a, int x)
{
    int i;
    for(i = 5; i >= 0;i--)
    {
        if(tab[x][i]=='.') {tab[x][i] = a; last.push(std::make_pair(x,i));return 0;}
    }
     return -1000000;
}

void reverse_addition(char tab[7][6])
{
    std::pair<int,int> buf = last.top();
    tab[buf.first][buf.second] = '.';
    last.pop();
}
int AI(char tab[7][6],char a,int depth, bool choosing_max,int max_depth)
{
    for(int i = 0; i< 7; i++)
    {
        if(tab[i][0]!= '.')continue;
        if(add(tab,a,i)==999999) { if(depth != max_depth)reverse_addition(tab); if(choosing_max) return 999999000; else return -999999000;}
        else reverse_addition(tab);
    }
    int score_array[7];
    int max_score,best_move;
        for(int i = 0; i < 7; i++)
        {
            if(tab[i][0]!= '.'){if(choosing_max)score_array[i] = -100000; else score_array[i] = 1000000; continue;}
            score_array[i] = add_AI(tab,a,i);
            if(score_array[i] != -1000000)
            {
                if(depth > 1)
                {
                    char b; if(a == 'O') b = 'X'; else b = 'O';
                    score_array[i] = AI(tab,b,depth-1,!choosing_max,max_depth);
                }
                else
                {
                    score_array[i] = score(tab,a);
                }
            }
            reverse_addition(tab);
        }
        srand(time(NULL));
        max_score = score_array[0];
        best_move = 0;
        for(int i = 0; i < 7; i++)
        {
            if(tab[i][0] != '.') continue;
            if(max_score == score_array[i]){if(rand() % 2){max_score = score_array[i]; best_move = i;}}
            if(choosing_max){ if(max_score<score_array[i]){max_score = score_array[i]; best_move = i;}}
            else {if(max_score>score_array[i]&& score_array[i] != -1000000){max_score = score_array[i]; best_move = i;}}
        }

        if(depth == max_depth) {return best_move;}

    return max_score;
}
int main()
{
    int max_depth = 5;
    char tab[7][6];
    int x,temp=1;
    char a;
    clear_board(tab);
    do
    {
        printf("Who goes first? 1 - You,else - CPU\n");
        scanf("%d",&temp);
        printf("If you want CPU to do your move, type -1\n");
        clear_board(tab);
        while(1)
            {
            if(temp==1){
            fflush(stdin);
            scanf("%d",&x);
            if(x == -1)
            {
                temp =  AI(tab,'X',max_depth,true,max_depth);
                printf("cpu move: %d\n",temp);
                if(temp==999999000)
                {
                    printf("Cheater won! \n"); print_board(tab); clear_board(tab); break;
                }else
                {
                    add(tab,'X',temp); print_board(tab);
                }
            }
            else
            if((add(tab,'X',x-1))==999999){printf("Player won! \n"); clear_board(tab);clear_board(tab); break; }
            }
            temp =  AI(tab,'O',max_depth,true,max_depth);
            if(temp==999999000)
            {
                printf("Computer won! \n"); print_board(tab); clear_board(tab); break;
            }else
            {
             add(tab,'O',temp); print_board(tab);
            }
            temp = 1;
        }
    printf("Do you want to play again? Y/N\n");
    fflush(stdin);
    scanf("%c",&a);
    }while(a == 'Y' || a == 'y');
    return 0;
}
