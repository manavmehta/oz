#include<iostream>
#include<stdio.h>

#define endl '\n'

using namespace std;

// split the input string to an array of args -> argv[] with additional NULL element
char **read_input(char *line){
    char **args = new char*;
    char *sep=" ", *token;
    int index=0;

    token = strtok(line, sep);
    while(token){
        args[index++]=token;
        token = strtok(NULL, sep);
    }
    args[index]=NULL;
    return args;
}


void printYes(){
    cout<<"Yes"<<endl;
}

int add(int a, int b){
    return a+b;
}