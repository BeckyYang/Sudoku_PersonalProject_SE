#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<string>
#include<fstream>
using namespace std;

ofstream  Output("Test.txt");
int main()
{

	char filename[100] = "D:\\SudokuTest.txt";
	FILE *fp1 = fopen(filename,"r+"); 
	char str[80];
	char* check;
	char rule[10][10] = {"13468","123456","0235678",//相应位置变为空格 
						"01234567","246","12345678",
						"0123568","234567","024578"};
	int rowline = 0;	
	while(fgets(str,20,fp1)){
		if(strcmp(str,"\n") == 0){
			continue;
		} 
		for(int i = 0; i < strlen(rule[rowline]); i++){
			str[2*(rule[rowline][i] - '0')] = '0';			
		}
		rowline++;
		Output << str;
		check = str + 1;
		if(rowline == 9  && fgets(check,20,fp1)){
			rowline = 0;
			Output << endl;
		}
		//cout << str << endl;
	}                                                                                                                                                                                                                                                                   
	return 0;
}
