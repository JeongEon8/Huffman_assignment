#pragma once
#include <iostream>
#include <cstring>

class Node {
public:
	int weight;
	int* nxt_ptr;
	int* prev_ptr;
};

// 입력 받은 문자열의 문자 빈도수를 반환하는 함수
void frequency(char input[10000]) {

}


// 빈도수를 내림차순으로 바꾸는 함수