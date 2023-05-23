// 2023 알고리즘(박병준 교수님)
// 과제명 : Huffman 코드를 이용한 파일압축(Compression) 프로그램 구현 및 실험
// 2020203066 김정언

#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <fstream>
#include <queue>
#include <stack>
#include <list>
#include <string>
#include <cstring>
#include <algorithm>

using namespace std;

# define MAX_LENGTH		1000

class Node {
public:
	Node();		// 생성자
	~Node();	// 소멸자

	void SetNode(char symbol, int frequency, Node* left, Node* right, Node* parent);	// 노드 정의하는 함수
	void ShowNode();	// 노드 정보 출력하는 함수

	char GetSymbol();	// 노드 Symbol 반환 함수
	int GetFrequency();	// 노드 frequency 반환 함수
	Node* GetLeftNode();	// 노드의 왼쪽 노드 반환 함수
	Node* GetRightNode();	// 노드의 오른쪽 노드 반환 함수
	Node* GetParentNode();	// 노드의 부모 노드 반환 함수

private:
	char symbol = NULL;
	int frequency = 0;
	Node* left = 0;
	Node* right = 0;
	Node* parent = 0;
};

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
queue<Node*> Node_queue; // 전역 node 큐 생성
list<Node*> Node_list;	// 전역 Node 리스트 생성
int depth = 0;	// 트리의 detpth를 저장할 전역 변수 생성
bool change = false;	// false -> 루트 기준 왼쪽 트리 탐색, true -> 루트 기준 오른쪽 트리 탐색

// 함수 선언부
void frequency(char arr[MAX_LENGTH], int enterkey); // 문자 빈도수를 반환하는 함수
void CountSymbol();	// 문자 개수를 세는 함수
void Ascending(); // 오름차순으로 바꿔주는 함수
Node* ShowLeft_leafNode(Node* ptr, Node* node, Node* parent_node, int parent_num, int n);	// 왼쪽 leaf 노드를 출력하는 함수
Node* ShowRight_leafNode(Node* ptr, Node* node, Node* parent_node, int parent_num, int n);// 오른쪽 leaf 노드를 출력하는 함수
void ShowTree(Node* node, Node* parent_node, int parent_num, int n); // 트리를 출력하는 함수
Node* ShowLeft_leafNode(Node* ptr, Node* node, Node* parent_node, int parent_num, int n);	// 왼쪽 leaf 노드 함수
Node* ShowRight_leafNode(Node* ptr, Node* node, Node* parent_node, int parent_num, int n);// 오른쪽 leaf 노드 함수
void Node_Huffman(Node* ptr, Node* node, Node* parent_node, int parent_num, int n, stack<int> huffman); // 노드의 허프만 코드를 만드는 함수

Node::Node() { }	// 생성자
Node::~Node() { }	// 소멸자

// 노드 정의하는 함수
void Node::SetNode(char symbol, int frequency, Node* left, Node* right, Node* parent) {
	this->symbol = symbol;
	this->frequency = frequency;
	this->left = left;
	this->right = right;
	this->parent = parent;
}

// 노드 정보 출력하는 함수
void Node::ShowNode() {
	printf("symbol: %c\tfrequency: %d\n", this->symbol, this->frequency);
}

// 노드 Symbol 반환 함수
char Node::GetSymbol() {
	return this->symbol;
}

// 노드 frequency 반환 함수
int Node::GetFrequency() {
	return this->frequency;
}

// 노드의 왼쪽 노드 반환 함수
Node* Node::GetLeftNode() {
	return this->left;
}

// 노드의 오른쪽 노드 반환 함수
Node* Node::GetRightNode() {
	return this->right;
}

// 노드의 부모 노드 반환 함수
Node* Node::GetParentNode() {
	return this->parent;
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

// 오름차순으로 바꿔주는 함수
void Ascending() {
	Node* node = new Node[Node_queue.size()];
	Node tmp;
	int length = Node_queue.size();

	for (int i = 0; i < length; i++) {
		// node[i].SetNode(Node_queue.front()->GetSymbol(), Node_queue.front()->GetFrequency(), Node_queue.front()->GetLeftNode(), Node_queue.front()->GetRightNode(), Node_queue.front()->GetParentNode());
		node[i] = *Node_queue.front();
		Node_queue.pop();
	}

	for (int i = length - 1; i > 0; i--) {
		for (int j = 0; j < i; j++) {
			if (node[j].GetFrequency() >= node[j + 1].GetFrequency()) {
				tmp = node[j];
				node[j] = node[j + 1];
				node[j + 1] = tmp;
			}
		}
	}

	for (int i = 0; i < length; i++) {
		Node_queue.push(&node[i]);
	}
}

// 왼쪽 leaf 노드를 출력하는 함수
Node* ShowLeft_leafNode(Node* ptr, Node* node, Node* parent_node, int parent_num, int n) {
	while (1) {
		if (ptr->GetLeftNode() == 0) {
			ptr->ShowNode();
			ptr = ptr->GetParentNode();
			for (int i = 0; i < parent_num; i++) {
				if (parent_node[i].GetFrequency() == ptr->GetFrequency() && parent_node[i].GetLeftNode() == ptr->GetLeftNode()) {
					ptr->SetNode(parent_node[i].GetSymbol(), parent_node[i].GetFrequency(), parent_node[i].GetLeftNode(), parent_node[i].GetRightNode(), parent_node[i].GetParentNode());
					Node_list.push_back(ptr);
					ptr->ShowNode();
					break;
				}
			}
			depth--;
			break;
		}
		else {
			ptr = ptr->GetLeftNode();
			if (ptr->GetSymbol() != NULL) {
				for (int i = 0; i < n; i++) {
					if (node[i].GetSymbol() == ptr->GetSymbol() && node[i].GetFrequency() == ptr->GetFrequency()) {
						ptr->SetNode(node[i].GetSymbol(), node[i].GetFrequency(), node[i].GetLeftNode(), node[i].GetRightNode(), node[i].GetParentNode());
						Node_list.push_back(ptr);
						break;
					}
				}
			}
			else {
				for (int i = 0; i < parent_num; i++) {
					if (parent_node[i].GetFrequency() == ptr->GetFrequency() && parent_node[i].GetLeftNode() == ptr->GetLeftNode()) {
						ptr->SetNode(parent_node[i].GetSymbol(), parent_node[i].GetFrequency(), parent_node[i].GetLeftNode(), parent_node[i].GetRightNode(), parent_node[i].GetParentNode());
						Node_list.push_back(ptr);
						ptr->ShowNode();
						break;
					}
				}
			}
			depth++;
		}
	}

	return ptr;
}

// 오른쪽 leaf 노드를 출력하는 함수
Node* ShowRight_leafNode(Node* ptr, Node* node, Node* parent_node, int parent_num, int n) {
	while (1) {
		if (ptr->GetRightNode() == 0) {
			printf("\t\t\t\t");
			ptr->ShowNode();
			int c_depth = depth;
			for (int i = 0; i < c_depth; i++) {
				ptr = ptr->GetParentNode();
				depth--;
				for (int j = 0; j < parent_num; j++) {
					if (parent_node[j].GetFrequency() == ptr->GetFrequency() && parent_node[j].GetRightNode() == ptr->GetRightNode()) {
						ptr->SetNode(parent_node[j].GetSymbol(), parent_node[j].GetFrequency(), parent_node[j].GetLeftNode(), parent_node[j].GetRightNode(), parent_node[j].GetParentNode());
						Node_list.push_back(ptr);
						ptr->ShowNode();
						break;
					}
				}
				list<Node*> find_list = Node_list;
				int length = find_list.size();
				int k;
				for (k = 0; k < length; k++) {
					if (ptr->GetRightNode() == 0) {
						break;
					}
					if (ptr->GetRightNode()->GetSymbol() == find_list.front()->GetSymbol() && ptr->GetRightNode()->GetFrequency() == find_list.front()->GetFrequency()) {
						break;
					}
					find_list.pop_front();
				}
				if (k == length) {
					ptr = ptr->GetRightNode();
					depth++;
					for (int j = 0; j < n; j++) {
						if (node[j].GetSymbol() == ptr->GetSymbol()) {
							ptr->SetNode(node[j].GetSymbol(), node[j].GetFrequency(), node[j].GetLeftNode(), node[j].GetRightNode(), node[j].GetParentNode());
							Node_list.push_back(ptr);
							break;
						}
					}
					for (int j = 0; j < parent_num; j++) {
						if (parent_node[j].GetFrequency() == ptr->GetFrequency() && parent_node[j].GetRightNode() == ptr->GetRightNode()) {
							ptr->SetNode(parent_node[j].GetSymbol(), parent_node[j].GetFrequency(), parent_node[j].GetLeftNode(), parent_node[j].GetRightNode(), parent_node[j].GetParentNode());
							Node_list.push_back(ptr);
							ptr->ShowNode();
							break;
						}
					}
					break;
				}
			}
			change = true;
			break;
		}
		else {
			ptr = ptr->GetRightNode();
			if (ptr->GetSymbol() != NULL) {
				for (int i = 0; i < n; i++) {
					if (node[i].GetSymbol() == ptr->GetSymbol()) {
						ptr->SetNode(node[i].GetSymbol(), node[i].GetFrequency(), node[i].GetLeftNode(), node[i].GetRightNode(), node[i].GetParentNode());
						Node_list.push_back(ptr);
						break;
					}
				}
			}
			else {
				for (int i = 0; i < parent_num; i++) {
					if (parent_node[i].GetFrequency() == ptr->GetFrequency() && parent_node[i].GetRightNode() == ptr->GetRightNode()) {
						ptr->SetNode(parent_node[i].GetSymbol(), parent_node[i].GetFrequency(), parent_node[i].GetLeftNode(), parent_node[i].GetRightNode(), parent_node[i].GetParentNode());
						Node_list.push_back(ptr);
						ptr->ShowNode();
						break;
					}
				}
			}
			depth++;
			if (ptr->GetSymbol() == NULL && ptr->GetFrequency() != 0) {
				break;
			}
		}
	}

	return ptr;
}

// 트리를 출력하는 함수
void ShowTree(Node* node, Node* parent_node, int parent_num, int n) {
	Node* ptr = &parent_node[parent_num - 1];
	Node_list.push_back(ptr);

	while (1) {
		if (depth == 0 && change == true) {
			break;
		}
		ptr = ShowLeft_leafNode(ptr, node, parent_node, parent_num, n);
		ptr = ShowRight_leafNode(ptr, node, parent_node, parent_num, n);
	}
}