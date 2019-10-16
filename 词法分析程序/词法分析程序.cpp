// 词法分析程序.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//============================================================================
// Name        : program.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include<iostream>

#include<fstream>

#include<cstdio>

#include<cstring>

#include<string>

#include<cstdlib>



using namespace std;



int aa;// fseek的时候用来接着的

string  word = "";

string  reserved_word[11];//保留

char buffer;//每次读进来的一个字符

int num = 0;//每个单词中当前字符的位置

int line = 1; //行数

int row = 1; //列数，就是每行的第几个

bool flag; //文件是否结束了

int flag2;//单词的类型



//设置保留字

void set_reserve()

{

	reserved_word[1] = "procedure";

	reserved_word[2] = "def";

	reserved_word[3] = "if";

	reserved_word[4] = "else";

	reserved_word[5] = "while";

	reserved_word[6] = "call";

	reserved_word[7] = "begin";

	reserved_word[8] = "end";

	reserved_word[9] = "and";

	reserved_word[10] = "or";

}



//看这个字是不是字母

bool judge_word(char x)

{

	if (x >= 'a' && x <= 'z' || x >= 'A' && x <= 'Z') {

		return true;

	}

	else return false;

}



//看这个字是不是数字

bool judge_number(char x)

{

	if (x >= '0' && x <= '9') {

		return true;

	}

	else return false;

}



//看这个字符是不是界符

bool judge_jiefu(char x)

{

	if (x == '(' || x == ')' || x == ',' || x == ';') {

		return true;

	}

	else return false;

}





//加减乘

bool judge_yunsuanfu1(char x)

{

	if (x == '+' || x == '-' || x == '*')

	{

		return true;

	}

	else return false;

}



//等于 赋值，大于小于 大于等于，小于等于，大于小于

bool judge_yunsuannfu2(char x)

{

	if (x == '=' || x == '>' || x == '<') {

		return true;

	}

	else return false;

}





//这个最大的函数的总体作用是从文件里读一个单词

int scan(FILE* fp)

{

	buffer = fgetc(fp);

	if (feof(fp)) {

		flag = 0; return 0;

	}

	//cout<<buffer;

	else if (buffer == ' ')

	{

		row++;

		return 0;

	}

	else if (buffer == '\n')

	{

		line++;

		row = 1;

		return 0;

	}



	//如果是字母开头或'_' 看关键字还是普通单词

	else if (judge_word(buffer) || buffer == '_')

	{

		word += buffer; row++;

		while ((buffer = fgetc(fp)) && (judge_word(buffer) || judge_number(buffer) || buffer == '_'))

		{

			word += buffer; row++;

		}

		if (feof(fp)) {

			flag = 0; return 1;

		}

		//这个函数的意义是 因为保留字不区分大小写 要把大写字母全变成小写再比较

		string temp = word;

		for (int j = 0; j < temp.length(); j++)

		{

			if (temp[j] >= 'A' && temp[j] <= 'Z')

			{

				temp[j] += 32;

			}

		}

		for (int i = 1; i <= 10; i++) {

			if (temp == reserved_word[i]) {

				aa = fseek(fp, -1, SEEK_CUR);

				return 3;

			}

		}

		aa = fseek(fp, -1, SEEK_CUR);

		return 1;

	}



	//开始是加减乘 一定是类型4

	else if (judge_yunsuanfu1(buffer))

	{

		word += buffer; row++;

		return 4;

	}



	//开始是数字就一定是数字

	else if (judge_number(buffer))

	{

		word += buffer; row++;

		while ((buffer = fgetc(fp)) && judge_number(buffer))

		{

			word += buffer; row++;

		}

		if (feof(fp)) {

			flag = 0; return 2;

		}

		aa = fseek(fp, -1, SEEK_CUR);

		return 2;

	}



	//检验界符

	else if (judge_jiefu(buffer))

	{

		word += buffer; row++;

		return 6;

	}



	//检验 <=、  >=、  <>、  == =、 <、>

	else if (judge_yunsuannfu2(buffer))

	{

		row++;

		word += buffer;

		if (buffer == '<')   //为了检验题目中的<> <=

		{

			buffer = fgetc(fp);

			if (buffer == '>' || buffer == '=')

			{

				word += buffer;

				row++;

				return 5;

			}

		}

		//检验  >= ==

		else {

			buffer = fgetc(fp);

			if (buffer == '=')

			{

				word += buffer;

				row++;

				return 5;

			}

		}

		if (feof(fp)) {

			flag = 0;

		}

		aa = fseek(fp, -1, SEEK_CUR);

		return 4;

	}



	//首字符是/ 有可能是除号 也有可能是注释

	else if (buffer == '/')

	{

		row++; word += buffer;

		buffer = fgetc(fp);

		//这种情况是除号

		if (buffer != '*' && buffer != '/')

		{

			aa = fseek(fp, -1, SEEK_CUR);

			return 4;

		}

		// 这一行剩下的全被注释了

		if (buffer == '/')

		{

			word.clear();

			while ((buffer = fgetc(fp)) && buffer != '\n' && !feof(fp))

			{

				//真的什么也没有做

			}

			if (feof(fp)) {

				flag = 0; return 0;

			}

			else {

				aa = fseek(fp, -1, SEEK_CUR);

			}

			line++; row = 1;

			return 0;

		}

		if (buffer == '*')

		{

			bool flag5 = 1;

			while (flag5)

			{

				word.clear();

				buffer = fgetc(fp);

				row++;

				if (buffer == '\n') { line++; row = 1; }

				if (buffer != '*')continue;

				else {

					buffer = fgetc(fp);

					row++; if (buffer == '\n') { line++; row = 1; }

					if (buffer == '/') {

						flag5 = 0;

					}

					else continue;

				}

				if (feof(fp)) { flag = 0; return 0; }

			}



		}



	}



	else {

		word += buffer;

		row++;

		return -1;

	}

}



int main()

{

	set_reserve();//设置保留字

	cout << "introduction" << endl;

	cout << "open " << "code.txt" << endl;



	cout << "press any key" << endl;

	system("pause");



	flag = 1;

	//ifstream a("需要解析的源代码.txt");

	FILE* fp;

	if (!(fp = fopen("01.txt", "r")))

	{

		cout << "not found the file or other error " << endl;

		flag = 0;

	}



	while (flag == 1)

	{

		//flag2 返回的类型

		flag2 = scan(fp);//反复调用函数提取单词



		if (flag2 == 1)

		{

			cout << "type:1 identifier      " << "line " << line << " row " << row - word.length() << "  " << word << endl;

			if (word.length() > 20)

				cout << "ERROR Identifier length cannot exceed 20 characters" << endl;

			word.clear();

		}

		else if (flag2 == 3)

		{

			cout << "type:3 reserved word   " << "line " << line << " row " << row - word.length() << "  " << word << endl;

			word.clear();

		}

		else if (flag2 == 4)

		{

			cout << "type:4 unary_operator  " << "line " << line << " row " << row - 1 << "  " << word << endl;

			word.clear();

		}

		else if (flag2 == 2)

		{

			cout << "type:2 positive number " << "line " << line << " row " << row - word.length() << "  " << word << endl;

			if (word[0] == '0')

				cout << "ERROR: The first digit cannot be 0!" << endl;

			word.clear();

		}

		else if (flag2 == 6)

		{

			cout << "type:6 Separator       " << "line " << line << " row " << row - 1 << "  " << word << endl;

			word.clear();

		}

		else if (flag2 == 5)

		{

			cout << "type:6 double_operator " << "line " << line << " row " << row - 2 << "  " << word << endl;

			word.clear();

		}

		//非法字符

		else if (flag2 == -1)

		{

			cout << "Illegal character      " << "line " << line << " row " << row - 1 << "  " << word << endl;

			word.clear();

		}

	}



	int a = fclose(fp);

	cout << "press e to close" << endl;

	char end;

	while (cin >> end && end != 'e') {

		cout << "只有e可以关闭" << endl;

	}

	return 0;

}



// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
