#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <string>
#include <locale>
using namespace std;

#define MaxSize 10000 //������������ ������ ����������� ��������

/*��������� ��� �������� ���� � ��������*/
struct trans_state {
	int _length;			//����� �������� �� ������ ��������� � ������
	int _link;				//������ 
	map<char,int> _next;	//������ ��������� (�����)
};

trans_state automaton[MaxSize * 2]; //���������� ���������
int start_state, last_state;//��������� � �������� ���������
int count_diff_subs = 0;

/*�������� �������� � ��������� ����������*/
void InitSuffixAutomaton() {
	start_state = last_state = 0;		//��������� � �������� ��������� �����
	automaton[0]._length = 0;				//���������� �������� ����� = 0
	automaton[0]._link = -1;				//������ ���������
	start_state++;						

	//���� ������� �������� ��� ��������� �����
	for (int i = 0; i < MaxSize * 2; i++)
		automaton[i]._next.clear();
}

/*�������� ����������� ��������*/
void CreateSuffixAutomaton(char c) {
	int cur = start_state++;
	int p;

	automaton[cur]._length = automaton[last_state]._length + 1; //����� �� ���������

	//�� ���������� � ������ �������� ��� ���
	for (p = last_state; p != -1 && !automaton[p]._next.count(c); p = automaton[p]._link) 
		automaton[p]._next[c] = cur;

	//���� �������� �� �������, ����� �� ���������� ����������
	if (p == -1)
		automaton[cur]._link = 0;
	//����  ������� ����, �� ��������������� �� ������� ���������
	else { 
		int q = automaton[p]._next[c];                      //��������� ���������, ���� ����� �������

		if (automaton[p]._length + 1 == automaton[q]._length)
			automaton[cur]._link = q;
		//������ ����� ���������
		else {
			int clone = start_state++; //����� ���������
			//�������� q � ����� ���������
			automaton[clone]._length = automaton[p]._length + 1;
			automaton[clone]._next = automaton[q]._next; 
			automaton[clone]._link = automaton[q]._link;

			for (; p != -1 && automaton[p]._next[c] == q; p = automaton[p]._link)
				automaton[p]._next[c] = clone;

			automaton[q]._link = automaton[cur]._link = clone;
		}
	}
	last_state = cur;
}

/*������� ��������� ��������*/
void Counter() {
	//�������� ������� �� ������� ��������� ���������
	for (int i = 1; i < last_state + 2; i++)
		count_diff_subs += automaton[i]._length - automaton[automaton[i]._link]._length;
	count_diff_subs--; //�������� �� ���������� 1(������ ���������- ���� ������)
}

/*�������� ������*/
int main() {
	setlocale(LC_ALL, "rus");
	ifstream ifs;
	ofstream ofs;
	string S;

	ifs.open("input.txt");
	getline(ifs, S);
	ifs.close();

	cout << S << endl;

	InitSuffixAutomaton();

	for (int i = 0; i < S.size(); i++) // �������� �������� �� ������ S
		CreateSuffixAutomaton(S[i]);

	Counter(); //�������

	ofs.open("output.txt");
	ofs << "���������� ��������(������� ���������-������ � ���������-������): " << count_diff_subs + 2 << endl;
	ofs << "���������� ��������(�� ������� ���������-������ � ���������-������): " << count_diff_subs << endl;
	ofs.close();

	system("output.txt");

	return 0;
}