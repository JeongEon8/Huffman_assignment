// 2023 �˰���(�ں��� ������)
// ������ : Huffman �ڵ带 �̿��� ���Ͼ���(Compression) ���α׷� ���� �� ����
// 2020203066 ������

#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <cstring>
#include <fstream>

using namespace std;

# define MAX_LENGTH		1000

int Symbol_Num = 69;	// ������ Symbol ���� ����
int Symbol_frequency[69] = { 0 };	// ������ Symbol ������ �󵵼�
// 0~25 : �빮�� ���ĺ�
// 26~51 : �ҹ��� ���ĺ�
// 52~61 : ����
// 62 : �����̽�(" "), 63 : ��ǥ(","), 64 : ��ħǥ("."), 65 : ����ǥ("!"),
// 66 : ����ǥ("?"), 67 : ������("-"), 68 : enterkey(CR)
char Symbol[69] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',	// Symbol
					'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
					'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
					'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
					'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
					'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
					'8', '9', ' ', ',', '.', '!', '?', '-', 'CR' };

class Node {
public:
	Node();		// ������
	~Node();	// �Ҹ���
	
	void SetNode(char symbol, int frequency, Node* left, Node* right);
	void ShowNode();

private:
	char symbol = NULL;
	int frequency = 0;
	Node* left = 0;
	Node* right = 0;
};

Node::Node() { }	// ������
Node::~Node() { }	// �Ҹ���

void Node::SetNode(char symbol, int frequency, Node* left, Node* right) {
	this->symbol = symbol;
	this->frequency = frequency;
	this->left = left;
	this->right = right;
}

void Node::ShowNode() {
	printf("symbol: %c\tfrequency: %d\n", this->symbol, this->frequency);
}

// ���� �󵵼��� ��ȯ�ϴ� �Լ�
void frequency(char arr[MAX_LENGTH], int enterkey) {
	int i = 0;

	while (arr[i] != NULL) {
		if (arr[i] >= 97 && arr[i] <= 122) {	// �ҹ��ڶ�� 0~25
			Symbol_frequency[arr[i] - 71]++;
		}
		else if (arr[i] >= 65 && arr[i] <= 90) {	// �빮�ڶ�� 26~51
			Symbol_frequency[arr[i] - 65]++;
		}
		else if (arr[i] == 63) {	// ����ǥ��� 66
			Symbol_frequency[arr[i] + 3]++;
		}
		else if (arr[i] >= 48 && arr[i] <= 57) {	// ���ڶ�� 52~61
			Symbol_frequency[arr[i] + 4]++;
		}
		else if (arr[i] == 46) {	// ��ħǥ��� 64
			Symbol_frequency[arr[i] + 18]++;
		}
		else if (arr[i] == 45) {	// �������̶�� 67
			Symbol_frequency[arr[i] + 22]++;
		}
		else if (arr[i] == 44) {	// ��ǥ��� 63
			Symbol_frequency[arr[i] + 19]++;
		}
		else if (arr[i] == 33) {	// ����ǥ��� 65
			Symbol_frequency[arr[i] + 32]++;
		}
		else if (arr[i] == 32) {	// �����̽���� 62
			Symbol_frequency[arr[i] + 30]++;
		}
		i++;
	}

	// enterkey ��� 68
	Symbol_frequency[68] = enterkey;
}

// ���� ������ ���� �Լ�
void CountSymbol() {
	for (int i = 0; i < 69; i++) {
		if (Symbol_frequency[i] == 0) {
			Symbol_Num--;
		}
	}
}
