// 2023 �˰���(�ں��� ������)
// ������ : Huffman �ڵ带 �̿��� ���Ͼ���(Compression) ���α׷� ���� �� ����
// 2020203066 ������

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
	Node();		// ������
	~Node();	// �Ҹ���

	void SetNode(char symbol, int frequency, Node* left, Node* right, Node* parent);	// ��� �����ϴ� �Լ�
	void ShowNode();	// ��� ���� ����ϴ� �Լ�

	char GetSymbol();	// ��� Symbol ��ȯ �Լ�
	int GetFrequency();	// ��� frequency ��ȯ �Լ�
	Node* GetLeftNode();	// ����� ���� ��� ��ȯ �Լ�
	Node* GetRightNode();	// ����� ������ ��� ��ȯ �Լ�
	Node* GetParentNode();	// ����� �θ� ��� ��ȯ �Լ�

private:
	char symbol = NULL;
	int frequency = 0;
	Node* left = 0;
	Node* right = 0;
	Node* parent = 0;
};

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
queue<Node*> Node_queue; // ���� node ť ����
list<Node*> Node_list;	// ���� Node ����Ʈ ����
int depth = 0;	// Ʈ���� detpth�� ������ ���� ���� ����
bool change = false;	// false -> ��Ʈ ���� ���� Ʈ�� Ž��, true -> ��Ʈ ���� ������ Ʈ�� Ž��

// �Լ� �����
void frequency(char arr[MAX_LENGTH], int enterkey); // ���� �󵵼��� ��ȯ�ϴ� �Լ�
void CountSymbol();	// ���� ������ ���� �Լ�
void Ascending(); // ������������ �ٲ��ִ� �Լ�
Node* ShowLeft_leafNode(Node* ptr, Node* node, Node* parent_node, int parent_num, int n);	// ���� leaf ��带 ����ϴ� �Լ�
Node* ShowRight_leafNode(Node* ptr, Node* node, Node* parent_node, int parent_num, int n);// ������ leaf ��带 ����ϴ� �Լ�
void ShowTree(Node* node, Node* parent_node, int parent_num, int n); // Ʈ���� ����ϴ� �Լ�
Node* ShowLeft_leafNode(Node* ptr, Node* node, Node* parent_node, int parent_num, int n);	// ���� leaf ��� �Լ�
Node* ShowRight_leafNode(Node* ptr, Node* node, Node* parent_node, int parent_num, int n);// ������ leaf ��� �Լ�
void Node_Huffman(Node* ptr, Node* node, Node* parent_node, int parent_num, int n, stack<int> huffman); // ����� ������ �ڵ带 ����� �Լ�

Node::Node() { }	// ������
Node::~Node() { }	// �Ҹ���

// ��� �����ϴ� �Լ�
void Node::SetNode(char symbol, int frequency, Node* left, Node* right, Node* parent) {
	this->symbol = symbol;
	this->frequency = frequency;
	this->left = left;
	this->right = right;
	this->parent = parent;
}

// ��� ���� ����ϴ� �Լ�
void Node::ShowNode() {
	printf("symbol: %c\tfrequency: %d\n", this->symbol, this->frequency);
}

// ��� Symbol ��ȯ �Լ�
char Node::GetSymbol() {
	return this->symbol;
}

// ��� frequency ��ȯ �Լ�
int Node::GetFrequency() {
	return this->frequency;
}

// ����� ���� ��� ��ȯ �Լ�
Node* Node::GetLeftNode() {
	return this->left;
}

// ����� ������ ��� ��ȯ �Լ�
Node* Node::GetRightNode() {
	return this->right;
}

// ����� �θ� ��� ��ȯ �Լ�
Node* Node::GetParentNode() {
	return this->parent;
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

// ������������ �ٲ��ִ� �Լ�
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

// ���� leaf ��带 ����ϴ� �Լ�
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

// ������ leaf ��带 ����ϴ� �Լ�
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

// Ʈ���� ����ϴ� �Լ�
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