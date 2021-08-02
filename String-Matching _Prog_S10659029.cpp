#include <iostream>
#include<fstream>
#include <Windows.h>
#include <vector>
#include <string>
#include<cmath>
#include<math.h>
#include <iomanip>
#include <algorithm>
using namespace std;

#define Max 1024

string text;
int n, m;
int Table[Max];
int GoodSuffixTable[Max];
vector <int> Address;

void brute_force(string input);
void Horspool(string pattern);
void shiftTable(string pattern);
void good_suffix_table(string pattern);
void boyer_moore(string pattern);
void computeAddress();

int main() {
	LARGE_INTEGER ThisTime, ThatTime, PinTime;
	ifstream fin("Input3.txt");
	string pattern;
	int i, index;

	//ŪInput
	getline(fin, text);

	cout << "Text:" << endl << text << endl << endl;

	cout << "Enter the pattern>>" ;
	cin >> pattern;

	m = pattern.length();
	n = text.length();

	/*************** Brute Force Algorithm ***************/
	cout << endl << "Brute force Algorithm:" << endl;

	//�p��ɶ�
	QueryPerformanceFrequency(&PinTime);
	QueryPerformanceCounter(&ThisTime);

	brute_force(pattern);
	QueryPerformanceCounter(&ThatTime);

	if (Address.size()>0)
	{
		cout << "pattern is found at index ";

		computeAddress();	//�P�_����r

		//���index
		for (int i = 0; i < Address.size(); i++)
			cout << Address[i] << " ";

		cout << endl;
	}
	else
		cout << "pattern is not found!" << endl;

	cout << "����ɶ��G " << (double)(ThatTime.QuadPart - ThisTime.QuadPart) * 1000 / PinTime.QuadPart << "�@��" << endl << endl;
	Address.clear();//�M�ŧ�쪺index��m


	/*************** Horspool's Algorithm ***************/
	cout << "Horspool's Algorithm:" << endl;

	//�p��ɶ�
	QueryPerformanceFrequency(&PinTime);
	QueryPerformanceCounter(&ThisTime);

	Horspool(pattern);
	QueryPerformanceCounter(&ThatTime);

	if (Address.size() > 0)
	{
		cout << "pattern is found at index ";

		computeAddress();	//�P�_����r

		//���index
		for (int i = 0; i < Address.size(); i++)
			cout << Address[i] << " ";

		cout << endl;
	}
	else
		cout << "pattern is not found!" << endl;

	cout << "����ɶ��G " << (double)(ThatTime.QuadPart - ThisTime.QuadPart) * 1000 / PinTime.QuadPart << "�@��" << endl << endl;
	Address.clear();//�M�ŧ�쪺index��m


	/*************** Boyer Moore Algorithm ***************/
	cout << "Boyer Moore Algorithm:" << endl;

	//�p��ɶ�
	QueryPerformanceFrequency(&PinTime);
	QueryPerformanceCounter(&ThisTime);

	boyer_moore(pattern);
	QueryPerformanceCounter(&ThatTime);

	if (Address.size() > 0)
	{
		cout << "pattern is found at index ";

		computeAddress();	//�P�_����r

		//���index
		for (int i = 0; i < Address.size(); i++)
			cout << Address[i] << " ";

		cout << endl;
	}
	else
		cout << "pattern is not found!" << endl;

	cout << "����ɶ��G " << (double)(ThatTime.QuadPart - ThisTime.QuadPart) * 1000 / PinTime.QuadPart << "�@��" << endl << endl;
	Address.clear();//�M�ŧ�쪺index��m

	system("pause");
	return 0;
}

void brute_force(string pattern) {

	for (int i = 0; i < text.length() - pattern.length(); i++)
	{
		int j;
		for (j = 0; j < pattern.length(); j++)
		{
			if (text[i + j] != pattern[j])
				break;			
		}
		if (j == pattern.length()) {
			Address.push_back(i);
		}
	}
}

void Horspool(string pattern) {
	int i, k, sign = 0;

	shiftTable(pattern);

	i = m - 1;

	while (i < n - 1)
	{
		k = 0;
		while ((k <= m - 1) && (pattern[m - 1 - k] == text[i - k])) {
			k = k + 1;
		}

		if (k == m) {
			Address.push_back(i - m + 1);
			i = i + m;
		}
		else {
			if (Table[unsigned char(text[i])])
			{
				i = Table[unsigned char(text[i])] + i;
			}
			else
			{
				i = i + m;
			}
		}
	}
}

void shiftTable(string pattern) {

	for (int i = 0; i < Max; i++) {
		Table[i] = m;
	}
	
	for (int i = 0; i < m - 1; i++)
	{
		Table[unsigned char(pattern[i])] = m - 1 - i;
	}

}

void good_suffix_table(string pattern) {
	int match_count = 0;
	bool compare;
	int k, j;

	for (int i = 1; i < m; i++)
	{
		j = 0;
		compare = false;
		for (; i + j < m; j++)
		{
			compare = true;
			if (pattern[m - 1 - j] == pattern[m - 1 - i - j])
			{
				if (match_count == j)
				{
					GoodSuffixTable[match_count] = i;
					match_count = match_count + 1;
				}
			}
			else
				break;
		}
		if (compare && i + j == m)
		{
			for (k = match_count; k < Max; k++)
				GoodSuffixTable[k] = i;
		}
	}
}

void boyer_moore(string pattern) {
	int i, k;

	good_suffix_table(pattern);

	i = m - 1;
	while (i <= n - 1)
	{
		k = 0;
		while (k <= m - 1 && pattern[m - 1 - k] == text[i - k])
			k = k + 1;

		if (k == m)
		{
			Address.push_back(i - m + 1);
			i = i + m;
		}
		else
		{
			int d1, d2;
			d1 = Table[unsigned char(text[i])] - k;
			if (d1 < 1)
				d1 = 1;

			if (k == 0)
				i = i + d1;
			else
			{
				d2 = GoodSuffixTable[k - 1];
				if (d1 > d2)
					i = i + d1;
				else
					i = i + d2;
			}
		}
	}
}

void computeAddress() {
	int index;

	for (int i = 0; i < Address.size(); i++)
	{
		index = Address[i];
		for (int j = 0; j < Address[i]; j++)
		{
			//�J�줤��r�e���A�Ĥ@�欰�t�A�n�A��h��index��1
			if (text[j] < 0) {
				index--;	//����r�ĤG�Ӥ��ݧP�_�������L
			}
		}
		Address[i] = index; //�s�����T����m�л\�쥻��index
	}
}