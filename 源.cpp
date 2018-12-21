#include<iostream>
#include<cstring>
#include<ctime>
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<fstream>
#include<vector>
#include<algorithm>
#define gong (row / 3 * 3 + column / 3)
using namespace std;

char Data[10] = { '1','2','3','4','5','6','7','8' }; //除了固定数字之外的八个数字 
char Sudoku[200000002];//存放最终要打印的数独数组 
long long int datap = 0;
//bool next_permutation(iterator start,iterator end);  //当当前序列不存在下个排列时，函数返回false，否则返回true 
ofstream  OutputBuildFile("Sudoku.txt");
ofstream  OutputSolveFile("Sudoku.txt");

#pragma region//生成数独部分
long long int NeedCount = 0;//需要生成的数独数量 
long long int NowCount = 0;//当前生成的数独数量
void ThreeRows(char* rule);
void BuildMap();
void BuildSudoku(char* rule1, char* rule2, char* rule3);
void WriteBuildSudoku();
#pragma endregion

#pragma region//求解数独部分
//char Flag[9][9];//代表数独在初始状态下的空白区域 
char Rule[3][10][10];//代表三个规则：用于存储每行、每列及每宫的数字占用情况。当值为 0 表示未被占用，值为 1 表示已被占用。 
char State[9][9];//用于存储数独的数字分布状态 
bool IsFirstSudoku = true;//是否为第一个数独 ，默认为是 
bool IsFindAns = false;//默认未找到答案
bool CheckRule(int row, int column, int num);//检查状态 
void ReadSudokuFile(int row, char* string);//读取存放数独问题的文件 
void SolveSudoku(int row, int column);
void WriteOneSudoku();//写入当前的一个数独 
void SetRule(int row, int column, int num, bool state);
#pragma endregion

void BuildMap() {
	char rule1[10][10] = { "036","063" };
	char rule2[10][10] = { "147","174","417","471","714","741" };
	char rule3[10][10] = { "258","285","528","582","825","852" };
	do {
		Data[8] = '9'; //插入固定数字到末尾，这里为（6+2）%9+1 = 9 
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 6; j++) {
				for (int k = 0; k < 6; k++) {
					BuildSudoku(rule1[i], rule2[j], rule3[k]);
					NowCount++;
					if (NowCount == NeedCount) {
						return;
					}
				}
			}
		}
	} while (next_permutation(Data, Data + 8));//next_permutation函数使用前需要对排列进行升序排序 ，否则只能找出该排列之后的全排列 
}
void ThreeRows(char* rule) {
	for (int i = 0; i < 3; i++) {
		Sudoku[datap++] = Data[(8 + rule[i] - '0') % 9];
		for (int j = 1; j < 17; j++) {
			Sudoku[datap++] = ' ';//相邻数字之间的空格 
			j++;
			Sudoku[datap++] = Data[((16 - j) / 2 + rule[i] - '0') % 9];//数组 
		}
		Sudoku[datap++] = '\n';//每一行数字的换行 
	}
}
void BuildSudoku(char* rule1, char* rule2, char* rule3) {
	ThreeRows(rule1);
	ThreeRows(rule2);
	ThreeRows(rule3);
	Sudoku[datap++] = '\n';
}
void WriteBuildSudoku() {
	OutputBuildFile << Sudoku;
}

void SetRule(int row, int column, int num, bool state) {
	Rule[0][row][num] = state;
	Rule[1][column][num] = state;
	Rule[2][gong][num] = state;
}
bool CheckRule(int row, int column, int num) {
	if (Rule[0][row][num] == 0 && Rule[1][column][num] == 0 && Rule[2][gong][num] == 0) {//如果在本行、本列、本宫，数字num都没有出现 
		return true;//那么可以填入 
	}
	else {
		return false;
	}
}
void WriteOneSudoku() {
	char writeString[1000];//保存当前读取到的一个数独问题解决方案 ，包括数字、空格和换行 
	int temp = 0;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			writeString[temp++] = State[i][j];
			if ((temp + 1) % 18 == 0) {
				writeString[temp++] = '\n';
			}
			else {
				writeString[temp++] = ' ';
			}
		}
	}
	writeString[temp++] = '\0';//加上字符串终止符，否则会报错“烫烫烫烫
	OutputSolveFile << writeString;//向SolveSudoku.txt文件写入读取到的当前一个数独问题的求解
	//memset(writeString, NULL, sizeof(writeString));
}
void SolveSudoku(int row, int column) {
	bool IsSearch = false;
	while (State[row][column] != '0') {//循环直到找到一个数独问题的空格 
		if (column < 8) {
			column++; //列号小于8则列号+1，检查本行下一位数字 
		}
		else {
			column = 0;//否则将列号置零，行号+1，检查下一行 
			row++;

		}
		if (row == 9) {//如果循环查找完毕都没有找到空格，表明数独的解已被找到 
			IsFindAns = true;
			return;
		}
	}
	for (int i = 1; i <= 9; i++) {
		if (CheckRule(row, column, i)) {//如果数字i在当前位置符合填入规则 ，将其填入此空格 
			SetRule(row, column, i, 1);//标明当前位置该数字 i 已被占用 
			State[row][column] = i + '0';//更新当前数独状态的数组 
			IsSearch = true;
			SolveSudoku(row, column);//递归调用SolveSudoku函数 ，查找下一个空格位置可能的解 
		}
		if (IsSearch) {//如果已被搜过，没有进入递归(没有找到可以填入的数字），那么开始回溯 
			IsSearch = false;
			if (IsFindAns) {
				return;
			}
			else {
				State[row][column] = '0';
				SetRule(row, column, i, 0);//将原本填入的数字擦掉，同时消掉该数字的占用标记 
			}
		}
	}
}
void ReadSudokuFile(int row, char* string) {
	for (int i = 0; i < 17; i++) {
		if (i % 2 == 0) {
			State[row][i / 2] = string[i];
			SetRule(row, i / 2, string[i] - '0', 1);
		}
	}
	/*	cout << "State:" << endl;
		for(int i= 0; i < 9; i++){
			for(int j = 0; j < 9; j++){
				cout << State[i][j];
			}
			cout << endl;
		} */
}

int main(int argc, char* argv[])
{
	clock_t start;
	clock_t finish;
	double duration;
	if (argc == 3 && strcmp(argv[1], "-c") == 0) {
		for (int i = 0; i < strlen(argv[2]); i++) {
			if (argv[2][i] >= '0' && argv[2][i] <= '9') {
				NeedCount *= 10;
				NeedCount += argv[2][i] - '0';
			}
			else {
				cout << "Input error! Please enter the number in right format!" << endl;
				return 0;
			}
		}

		cout << "Great! Now we begin to create sudoku!" << endl;
		cout << "Needcount:" << NeedCount << endl;

		start = clock();
		while (NowCount < NeedCount) {
			BuildMap();
		}
		Sudoku[datap++] = '\n';//每个组数独之后的换行 
		WriteBuildSudoku();
		finish = clock();
		duration = (double)(finish - start) / CLOCKS_PER_SEC;
		cout << "You use " << duration << " seconds to build these sudoku!" << endl;
	}
	else if (argc == 3 && strcmp(argv[1], "-s") == 0) {
		cout << "Oh!Let's start to get these sudoku's solutions!" << endl;
		FILE *fp1 = fopen(argv[2], "r");
		char string[100];
		int rowline = 0;
		start = clock();
		while (fgets(string, 20, fp1)) {
			if (strcmp(string, "\n") == 0) {
				continue;
			}
			ReadSudokuFile(rowline, string);
			rowline++;
			if (rowline == 9) {
				if (!IsFirstSudoku) {//不是第一个数独问题则打印一个换行
					OutputSolveFile << endl;
				}
				else {
					IsFirstSudoku = false;
				}
				IsFindAns = false;
				SolveSudoku(0,0);
				rowline = 0;
				WriteOneSudoku();
				memset(Rule, 0, sizeof(Rule));//每次求解完一个数独后将Rule重置归零 
			}
		}
		finish = clock();
		duration = (double)(finish - start) / CLOCKS_PER_SEC;
		cout << "You use " << duration << " seconds to solve these sudoku!" << endl;
	}
	else {
		cout << "Input error! Wrong format!" << endl;
	}
	system("pause");
	return 0;
}