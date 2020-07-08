#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <string>
#include <locale>
using namespace std;

#define MaxSize 10000 //максимальный размер суффиксного автомата

/*структура для хранения инфо о переходе*/
struct trans_state {
	int _length;			//длина перехода из одного состояния в другое
	int _link;				//ссылка 
	map<char,int> _next;	//список переходов (ребра)
};

trans_state automaton[MaxSize * 2]; //количество состояний
int start_state, last_state;//начальное и конечное состояние
int count_diff_subs = 0;

/*создание автомата с начальным состоянием*/
void InitSuffixAutomaton() {
	start_state = last_state = 0;		//начальное и конечное состояния равны
	automaton[0]._length = 0;				//начального перехода длина = 0
	automaton[0]._link = -1;				//сыллка состояния
	start_state++;						

	//если автомат строится для различных строк
	for (int i = 0; i < MaxSize * 2; i++)
		automaton[i]._next.clear();
}

/*создание суффиксного автомата*/
void CreateSuffixAutomaton(char c) {
	int cur = start_state++;
	int p;

	automaton[cur]._length = automaton[last_state]._length + 1; //длина до состояния

	//от последнего и такого элемента еще нет
	for (p = last_state; p != -1 && !automaton[p]._next.count(c); p = automaton[p]._link) 
		automaton[p]._next[c] = cur;

	//если перехода не нашлось, дошли до фиктивного стостояния
	if (p == -1)
		automaton[cur]._link = 0;
	//если  переход есть, то останавливаемся на текущем состоянии
	else { 
		int q = automaton[p]._next[c];                      //обозначим состояние, куда ведет переход

		if (automaton[p]._length + 1 == automaton[q]._length)
			automaton[cur]._link = q;
		//создаём новое состояние
		else {
			int clone = start_state++; //новое состояние
			//копируем q в новое состояние
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

/*подсчёт различных подстрок*/
void Counter() {
	//начинаем считать не включая начальное состояние
	for (int i = 1; i < last_state + 2; i++)
		count_diff_subs += automaton[i]._length - automaton[automaton[i]._link]._length;
	count_diff_subs--; //отнимает от количества 1(входит подстрока- сама строка)
}

/*основная фкнция*/
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

	for (int i = 0; i < S.size(); i++) // создание автомата на основе S
		CreateSuffixAutomaton(S[i]);

	Counter(); //считаем

	ofs.open("output.txt");
	ofs << "Количество подстрок(включая подстроку-строка и подстроку-пустую): " << count_diff_subs + 2 << endl;
	ofs << "Количество подстрок(не включая подстроку-строка и подстроку-пустую): " << count_diff_subs << endl;
	ofs.close();

	system("output.txt");

	return 0;
}