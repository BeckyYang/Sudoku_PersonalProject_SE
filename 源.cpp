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

char Data[10] = { '1','2','3','4','5','6','7','8' }; //���˹̶�����֮��İ˸����� 
char Sudoku[200000002];//�������Ҫ��ӡ���������� 
long long int datap = 0;
//bool next_permutation(iterator start,iterator end);  //����ǰ���в������¸�����ʱ����������false�����򷵻�true 
ofstream  OutputBuildFile("Sudoku.txt");
ofstream  OutputSolveFile("Sudoku.txt");

#pragma region//������������
long long int NeedCount = 0;//��Ҫ���ɵ��������� 
long long int NowCount = 0;//��ǰ���ɵ���������
void ThreeRows(char* rule);
void BuildMap();
void BuildSudoku(char* rule1, char* rule2, char* rule3);
void WriteBuildSudoku();
#pragma endregion

#pragma region//�����������
//char Flag[9][9];//���������ڳ�ʼ״̬�µĿհ����� 
char Rule[3][10][10];//���������������ڴ洢ÿ�С�ÿ�м�ÿ��������ռ���������ֵΪ 0 ��ʾδ��ռ�ã�ֵΪ 1 ��ʾ�ѱ�ռ�á� 
char State[9][9];//���ڴ洢���������ֲַ�״̬ 
bool IsFirstSudoku = true;//�Ƿ�Ϊ��һ������ ��Ĭ��Ϊ�� 
bool IsFindAns = false;//Ĭ��δ�ҵ���
bool CheckRule(int row, int column, int num);//���״̬ 
void ReadSudokuFile(int row, char* string);//��ȡ�������������ļ� 
void SolveSudoku(int row, int column);
void WriteOneSudoku();//д�뵱ǰ��һ������ 
void SetRule(int row, int column, int num, bool state);
#pragma endregion

void BuildMap() {
	char rule1[10][10] = { "036","063" };
	char rule2[10][10] = { "147","174","417","471","714","741" };
	char rule3[10][10] = { "258","285","528","582","825","852" };
	do {
		Data[8] = '9'; //����̶����ֵ�ĩβ������Ϊ��6+2��%9+1 = 9 
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
	} while (next_permutation(Data, Data + 8));//next_permutation����ʹ��ǰ��Ҫ�����н����������� ������ֻ���ҳ�������֮���ȫ���� 
}
void ThreeRows(char* rule) {
	for (int i = 0; i < 3; i++) {
		Sudoku[datap++] = Data[(8 + rule[i] - '0') % 9];
		for (int j = 1; j < 17; j++) {
			Sudoku[datap++] = ' ';//��������֮��Ŀո� 
			j++;
			Sudoku[datap++] = Data[((16 - j) / 2 + rule[i] - '0') % 9];//���� 
		}
		Sudoku[datap++] = '\n';//ÿһ�����ֵĻ��� 
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
	if (Rule[0][row][num] == 0 && Rule[1][column][num] == 0 && Rule[2][gong][num] == 0) {//����ڱ��С����С�����������num��û�г��� 
		return true;//��ô�������� 
	}
	else {
		return false;
	}
}
void WriteOneSudoku() {
	char writeString[1000];//���浱ǰ��ȡ����һ���������������� ���������֡��ո�ͻ��� 
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
	writeString[temp++] = '\0';//�����ַ�����ֹ��������ᱨ����������
	OutputSolveFile << writeString;//��SolveSudoku.txt�ļ�д���ȡ���ĵ�ǰһ��������������
	//memset(writeString, NULL, sizeof(writeString));
}
void SolveSudoku(int row, int column) {
	bool IsSearch = false;
	while (State[row][column] != '0') {//ѭ��ֱ���ҵ�һ����������Ŀո� 
		if (column < 8) {
			column++; //�к�С��8���к�+1����鱾����һλ���� 
		}
		else {
			column = 0;//�����к����㣬�к�+1�������һ�� 
			row++;

		}
		if (row == 9) {//���ѭ��������϶�û���ҵ��ո񣬱��������Ľ��ѱ��ҵ� 
			IsFindAns = true;
			return;
		}
	}
	for (int i = 1; i <= 9; i++) {
		if (CheckRule(row, column, i)) {//�������i�ڵ�ǰλ�÷���������� ����������˿ո� 
			SetRule(row, column, i, 1);//������ǰλ�ø����� i �ѱ�ռ�� 
			State[row][column] = i + '0';//���µ�ǰ����״̬������ 
			IsSearch = true;
			SolveSudoku(row, column);//�ݹ����SolveSudoku���� ��������һ���ո�λ�ÿ��ܵĽ� 
		}
		if (IsSearch) {//����ѱ��ѹ���û�н���ݹ�(û���ҵ�������������֣�����ô��ʼ���� 
			IsSearch = false;
			if (IsFindAns) {
				return;
			}
			else {
				State[row][column] = '0';
				SetRule(row, column, i, 0);//��ԭ����������ֲ�����ͬʱ���������ֵ�ռ�ñ�� 
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
		Sudoku[datap++] = '\n';//ÿ��������֮��Ļ��� 
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
				if (!IsFirstSudoku) {//���ǵ�һ�������������ӡһ������
					OutputSolveFile << endl;
				}
				else {
					IsFirstSudoku = false;
				}
				IsFindAns = false;
				SolveSudoku(0,0);
				rowline = 0;
				WriteOneSudoku();
				memset(Rule, 0, sizeof(Rule));//ÿ�������һ��������Rule���ù��� 
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