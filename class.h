// 2023 알고리즘(박병준 교수님)
// 과제명 : Huffman 코드를 이용한 파일압축(Compression) 프로그램 구현 및 실험
// 2020203066 김정언

#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <cstring>
#include <fstream>

using namespace std;

# define MAX_LENGTH		1000

int Symbol_Num = 69;	// 등장한 Symbol 종류 개수
int Symbol_frequency[69] = { 0 };	// 등장한 Symbol 종류별 빈도수
// 0~25 : 대문자 알파벳
// 26~51 : 소문자 알파벳
// 52~61 : 숫자
// 62 : 스페이스(" "), 63 : 쉼표(","), 64 : 마침표("."), 65 : 느낌표("!"),
// 66 : 물음표("?"), 67 : 하이픈("-"), 68 : enterkey(CR)
char Symbol[69] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',	// Symbol
					'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
					'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
					'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
					'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
					'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
					'8', '9', ' ', ',', '.', '!', '?', '-', 'CR' };

class Node {
public:
	Node();		// 생성자
	~Node();	// 소멸자
	
	void SetNode(char symbol, int frequency, Node* left, Node* right);
	void ShowNode();

private:
	char symbol = NULL;
	int frequency = 0;
	Node* left = 0;
	Node* right = 0;
};

Node::Node() { }	// 생성자
Node::~Node() { }	// 소멸자

void Node::SetNode(char symbol, int frequency, Node* left, Node* right) {
	this->symbol = symbol;
	this->frequency = frequency;
	this->left = left;
	this->right = right;
}

void Node::ShowNode() {
	printf("symbol: %c\tfrequency: %d\n", this->symbol, this->frequency);
}

// 문자 빈도수를 반환하는 함수
void frequency(char arr[MAX_LENGTH], int enterkey) {
	int i = 0;

	while (arr[i] != NULL) {
		if (arr[i] >= 97 && arr[i] <= 122) {	// 소문자라면 0~25
			Symbol_frequency[arr[i] - 71]++;
		}
		else if (arr[i] >= 65 && arr[i] <= 90) {	// 대문자라면 26~51
			Symbol_frequency[arr[i] - 65]++;
		}
		else if (arr[i] == 63) {	// 물음표라면 66
			Symbol_frequency[arr[i] + 3]++;
		}
		else if (arr[i] >= 48 && arr[i] <= 57) {	// 숫자라면 52~61
			Symbol_frequency[arr[i] + 4]++;
		}
		else if (arr[i] == 46) {	// 마침표라면 64
			Symbol_frequency[arr[i] + 18]++;
		}
		else if (arr[i] == 45) {	// 하이픈이라면 67
			Symbol_frequency[arr[i] + 22]++;
		}
		else if (arr[i] == 44) {	// 쉼표라면 63
			Symbol_frequency[arr[i] + 19]++;
		}
		else if (arr[i] == 33) {	// 느낌표라면 65
			Symbol_frequency[arr[i] + 32]++;
		}
		else if (arr[i] == 32) {	// 스페이스라면 62
			Symbol_frequency[arr[i] + 30]++;
		}
		i++;
	}

	// enterkey 라면 68
	Symbol_frequency[68] = enterkey;
}

// 문자 개수를 세는 함수
void CountSymbol() {
	for (int i = 0; i < 69; i++) {
		if (Symbol_frequency[i] == 0) {
			Symbol_Num--;
		}
	}
}
