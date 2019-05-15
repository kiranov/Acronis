/*Решение задачи с использованием алгоритма Бойера-Мура за O(n+m)*/
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
using namespace std;

int SIZE = 10;

//вычисление Z-функции
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
//Печать накопленного в буффер.
void print(char * buff, int size, int * suff, int count, int m){
	int l = 0;
	for(int i = m; i < size; i++){
		cout << buff[i];
		if(l < count && i == suff[l]){
			cout << "*";
			l++;
		}
	}
}

int main(int argc, char **argv)
{
	int pat_len = strlen(argv[1]);
	if(SIZE<2*pat_len){
		SIZE=pat_len*2;
	}
	char * str = (char*)malloc(sizeof(char)*(pat_len+1));
	strcpy(str, argv[1]);
	char * str2 = (char*)malloc(sizeof(char)*(pat_len+1));
	for(int i = 0; i < pat_len; i++){
		str2[i] = str[pat_len-i-1];
	}
	str2[pat_len] = '\0';
	//вычиление таблицы суффиксов
	vector<int> z = z_function(str2);
	vector<int> suffshift(pat_len + 1, pat_len);
	for (int j = pat_len - 1; j > 0; j--) {
		suffshift[pat_len - z[j]] = j;
	}
	for (int j = 1, r = 0; j <= pat_len - 1; j++){
		if (j + z[j] == pat_len)
		for (; r <= j; r++)
			if(suffshift[r] == pat_len) suffshift[r] = j;
	}
	int suff[SIZE+pat_len];
	int count = 0;
	char * buff = (char*)malloc(sizeof(char)*(SIZE));
	int j, strlen, bound = 0, f_len;
	//первый раз читаем строку вне цикла, так как в начале буффера не нужно хранить конец предыдущей строки
	if((strlen = read(1, buff, SIZE)) < 0){
		perror("read_1");
		exit(1);
	}
	//счетчик, отвечающий за корректную работу на стыке
	f_len = strlen;
	while(1){
		//вычисление позиций, на которые будет поставлена *
		for (int i = 0; i <= SIZE - pat_len; i += suffshift[j+1]) {
			for (j = pat_len - 1; j >= bound && str[j] == buff[i + j]; j--);
			if (j < bound) {
			 suff[count] = i+pat_len-1;
			 count++;
			 bound = pat_len - suffshift[0];
			 j = -1;
			} else {
			 bound = 0;
			}
		}
		//печать из буффера со стартовой позиции f_str-strlen
		print(buff, f_len, suff, count, f_len-strlen);
		if(f_len < SIZE){
			break;
		}
		//копирование конца строки(последних m-1 символов)
		for(int z = 0; z < pat_len-1; z++){
			 buff[pat_len-z-2] = buff[f_len-z-1];
		}
		if((strlen = read(1, buff+pat_len-1, f_len-pat_len+1)) < 0){
			perror("read");
			exit(1);
		}
		f_len = strlen + pat_len - 1;
		count = 0;
	}
	free(buff);
	free(str);
	free(str2);
	return 0;
}
