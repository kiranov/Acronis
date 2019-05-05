/*Для решения данной задачи мною был выбран алгоритм Бойера-Мура, несмотря на то, в курсе ТРЯП нам предлагался алгоритм Кнута-Морриса-Пратта, так как алгоритм Бойера Мура считается асимптотически
 * более быстрым алгоритомом. Изучив материал на wikipedia, думал написать алгоритм использующий таблицу стоп символов и суффиксов, однако он работает за O(m+n+|Z|), где n,m,|Z| - длина паттерна, текста
 * и мощность алфавита соответственно, выяснилось, что используя правило Галилея(https://ru.wikipedia.org/wiki/%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%91%D0%BE%D0%B9%D0%B5%D1%80%D0%B0_%E2%80%94_%D0%9C%D1%83%D1%80%D0%B0)
 * можно получить ассимптотику O(m+n)*/
/*также вывод cout может быть заменен на какой-нибудь printf длу увелечения скорости
*/


#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <unistd.h>
using namespace std;

int SIZE = 10; //размер buff взят небольшим для того, чтобы проверить работу на стыке слов, на деле размер можно брать 4096(размер страницы)

//таблица стоп символов, которая в итоге не пригодилась
int * table1(string str){
	int * arr = (int*)malloc(sizeof(int)*256);
	int len = str.length();
	for(int i = 0; i < 256; i++){
		arr[i] = len;
	}
	char a;
	for(int i = len-1; i >= 0 ; i--){
		a = (char)str[i];
		//cout << a << endl;
		if(arr[(int)a] != len){
		}
		else{
			arr[(int)a] = i+1;
		}
	}
	return arr;
}


//Z-функция - вспомогательная функция для вычисления таблицы суффиксов
vector<int> z_function (char * s) {
	int n = (int) strlen(s);
	vector<int> z (n);
	for (int i=1, l=0, r=0; i<n; ++i) {
		if (i <= r)
			z[i] = min (r-i+1, z[i-l]);
		while (i+z[i] < n && s[z[i]] == s[i+z[i]])
			++z[i];
		if (i+z[i]-1 > r)
			l = i,  r = i+z[i]-1;
	}
	return z;
}


int main(int argc, char **argv)
{
	int m = strlen(argv[1]);
	char * str = (char*)malloc(sizeof(char)*(m+2));
	strcpy(str, argv[1]);
	char * str2 = (char*)malloc(sizeof(char)*(m+1));
	for(int i = 0; i < m; i++){
		str2[i] = str[m-i-1];
	}
	str2[m] = '\0';
	//вычисление Z функции для перевернутого слова
	vector<int> z = z_function(str2);
	//таблица суффиксов
	vector<int> suffshift(m + 1, m);
	 for (int j = m - 1; j > 0; j--) {
		 suffshift[m - z[j]] = j;
	 }
	 for (int j = 1, r = 0; j <= m - 1; j++)
		if (j + z[j] == m)
		 for (; r <= j; r++)
			if (suffshift[r] == m) suffshift[r] = j;
	//buff для теста
	char * buff = (char*)malloc(sizeof(char)*(SIZE+m));
	int j = 0, bound = 0, strlen, i = 0, move = 0, param = 0, t = 0;
	//в цикле считывается текст, при этом отслеживается наложение слов, также в начало буффера после обаботки записываются m-1 символ из конца, чтобы корректно обрабатывался стык
	while((strlen = read(1, buff+param, SIZE)) > 0){
		for (i = 0; i <= strlen+param - m; i += suffshift[j+1]) {
			for (j = m - 1; (j >= bound && str[j] == buff[i + j]); j--){};
			if(j < bound){
				//обаботка стыка
				if(i < param){
					for(int k = param+t; k < m + i; k++){
						cout << buff[k];
					}
					t = m + i - param;
					cout << "*";
					move = i+m;
					bound = m - suffshift[0];
					j = -1;
				}
				//обработка оставшегося текста(включая случай наложения)
				else{
					for(int k = move; k < i; k++){
						cout << buff[k];
					}
					
					if(move > i){
						for(int k = move; k < move+(move-i); k++){
							cout << buff[k];
						}
						cout << "*";
						move = 2*move-i;
					}
					else{
						for(int k = i; k < i+m; k++){
							cout << buff[k];
						}
						cout << "*";
						move = i+m;
					}
					bound = m - suffshift[0];
					j = -1;
				}
			} 
			else {
				bound = 0;
			}
		}	
		//вывод остатка после обаботки
			for(int k = move; k < strlen+param; k++){
					cout << buff[k];
			}
			move = param;
			j = 0;
			t = 0;
		//перезаписываем начало
			for(int k = 0; k < m-1; k++){
				buff[k] = buff[param+SIZE-m+1+k];
			}
			param=m-1;
		if(strlen < SIZE){
			break;
		}
	}
	free(buff);
	free(str);
	free(str2);
	return 0;
}
