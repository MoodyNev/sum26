#include <stdio.h>
char lol[] = "101010100";
int F[] = {1,0,0,0,1,0,0};
int delta[][2]={{0,1},{3,6},{3,4},{2,5},{0,6},{2,6},{6,6}};

int drei_not_11(char *w){
    int q = 0;
    while(*w) q = delta[q][*w++-'0'];
    return F[q];
}

int main(void){
    drei_not_11(*lol);
}

