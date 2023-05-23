// 2023 �˰���(�ں��� ������)
// ������ : Huffman �ڵ带 �̿��� ���Ͼ���(Compression) ���α׷� ���� �� ����
// 2020203066 ������

#include "class.h"
using namespace std;

int main(void) {
	FILE* inputFile = fopen("test.txt", "r");	// �Է� ���� �б� ���� ��
	int enterkey = -1;	// enterkey ������ ���� ����

	// ������ ���� ��� ���α׷� ����
	if (inputFile == NULL) {
		printf("������ �����ϴ�. ���α׷��� �����մϴ�.\n");
		return 0;
	}

	// ������ �� �پ� �о��
	while (1) {
		char arr[MAX_LENGTH];
		char* ptr = fgets(arr, MAX_LENGTH, inputFile);

		// ���� ��
		if (ptr == NULL) {
			break;
		}

		enterkey++;
		frequency(arr, enterkey);
	}
	
	CountSymbol();

	Node* node = new Node[Symbol_Num];	// node �迭 ����
	Node* parent_node = new Node[Symbol_Num];	// parent_node �迭 ����
	int n = 0;	// node �迭�� �ϳ��� �����ϱ� ���� ����
	int parent_num = 0;	// �θ� node �迭�� ��ġ�� �˷��ִ� ����
	stack<int> huffman;	// ������ �ڵ带 ����� ���� stack

	// ��� ����
	for (int i = 0; i < 69; i++) {
		int tmp = 0;

		for (int j = 0; j < 69; j++) {
			if (Symbol_frequency[j] == 0) {	// �ѹ��� ������ ���� ������ ��
				continue;
			}

			if (Symbol_frequency[tmp] <= Symbol_frequency[j]) { // ���� �󵵼� ū ��ġ�� ã��
				tmp = j;
			}
		}

		if (Symbol_frequency[tmp] != 0) {	// ��� ����
			node[n].SetNode(Symbol[tmp], Symbol_frequency[tmp], 0, 0, 0);
			Node_queue.push(&node[n]);	// list�� ������������ ��� ����
			Symbol_frequency[tmp] = 0;
			n++;
		}
	}

	printf("�����\n");
	queue<Node*> test = Node_queue;
	int length = test.size();
	for (int i = 0; i < length; i++) {
		test.front()->ShowNode();
		test.pop();
	}
	printf("\n");

	// ������ �ڵ忡 ���� Ʈ�� ����
	while (!Node_queue.empty()) {
		Node* node1 = 0;
		Node* node2 = 0;

		Ascending();	// node ť ������������ ����

		// �󵵼��� ���� ���� 2�� ����
		if (Node_queue.size() != 1) {
			node1 = Node_queue.front();
			for (int i = 0; i < n; i++) {
				if (node[i].GetSymbol() == node1->GetSymbol()) {
					node1->SetNode(node[i].GetSymbol(), node[i].GetFrequency(), node[i].GetLeftNode(), node[i].GetRightNode(), node[i].GetParentNode());
					break;
				}
			}
			Node_queue.pop();
			node2 = Node_queue.front();
			for (int i = 0; i < n; i++) {
				if (node[i].GetSymbol() == node2->GetSymbol()) {
					node2->SetNode(node[i].GetSymbol(), node[i].GetFrequency(), node[i].GetLeftNode(), node[i].GetRightNode(), node[i].GetParentNode());
					break;
				}
			}
			Node_queue.pop();
		}
		else {
			Node_queue.pop();
			break;
		}

		// ��� 2���� ��ħ
		parent_node[parent_num].SetNode(NULL, node1->GetFrequency() + node2->GetFrequency(), node1, node2, 0);
		Node_queue.push(&parent_node[parent_num]);
		for (int i = 0; i < n; i++) {
			if (node1->GetSymbol() == node[i].GetSymbol()) {
				node[i].SetNode(node[i].GetSymbol(), node[i].GetFrequency(), node[i].GetLeftNode(), node[i].GetRightNode(), &parent_node[parent_num]);
			}
			if (node2->GetSymbol() == node[i].GetSymbol()) {
				node[i].SetNode(node[i].GetSymbol(), node[i].GetFrequency(), node[i].GetLeftNode(), node[i].GetRightNode(), &parent_node[parent_num]);
			}
		}

		for (int i = 0; i < parent_num; i++) {
			if (node1->GetLeftNode() != 0) {
				if (parent_node[i].GetFrequency() == node1->GetFrequency() && parent_node[i].GetLeftNode()->GetSymbol() == node1->GetLeftNode()->GetSymbol()) {
					parent_node[i].SetNode(parent_node[i].GetSymbol(), parent_node[i].GetFrequency(), parent_node[i].GetLeftNode(), parent_node[i].GetRightNode(), &parent_node[parent_num]);
				}
			}
			if (node2->GetLeftNode() != 0) {
				if (parent_node[i].GetFrequency() == node2->GetFrequency() && parent_node[i].GetLeftNode()->GetSymbol() == node2->GetLeftNode()->GetSymbol()) {
					parent_node[i].SetNode(parent_node[i].GetSymbol(), parent_node[i].GetFrequency(), parent_node[i].GetLeftNode(), parent_node[i].GetRightNode(), &parent_node[parent_num]);
				}
			}
		}
		parent_num++;
	}

	// Ʈ�� ���
	ShowTree(node, parent_node, parent_num, n);
	
	/*
	int zero = 0;
	int one = 1;
	string** huffman_code = new string*[n];
	for (int i = 0; i < n; i++) {
		huffman_code[i] = new string[1000];
	}

	for (int j = 0; j < n; j++) {
		Node* ptr = &node[j];

		while (ptr->GetFrequency() != parent_node[parent_num-1].GetFrequency()) {
			for (int i = 0; i < parent_num; i++) {
				if (ptr->GetSymbol() == parent_node[i].GetLeftNode()->GetSymbol() && ptr->GetFrequency() == parent_node[i].GetLeftNode()->GetFrequency()) {
					huffman.push(zero);
					ptr = ptr->GetParentNode();
					for (int i = 0; i < parent_num; i++) {
						if (parent_node[i].GetFrequency() == ptr->GetFrequency()) {
							ptr->SetNode(parent_node[i].GetSymbol(), parent_node[i].GetFrequency(), parent_node[i].GetLeftNode(), parent_node[i].GetRightNode(), parent_node[i].GetParentNode());
							break;
						}
					}
				}		
				else if (ptr->GetSymbol() == parent_node[i].GetRightNode()->GetSymbol() && ptr->GetFrequency() == parent_node[i].GetRightNode()->GetFrequency()) {
					huffman.push(one);
					ptr = ptr->GetParentNode();
					for (int i = 0; i < parent_num; i++) {
						if (parent_node[i].GetFrequency() == ptr->GetFrequency()) {
							ptr->SetNode(parent_node[i].GetSymbol(), parent_node[i].GetFrequency(), parent_node[i].GetLeftNode(), parent_node[i].GetRightNode(), parent_node[i].GetParentNode());
							break;
						}
					}
				}
			}
		}

		while (!huffman.empty()) {
			huffman_code[j]->append(to_string(huffman.top()));
			huffman.pop();
		}
	}
	
	char character;
	character = getc(inputFile);
	character = getc(inputFile);
	for (int i = 0; i < n; i++) {
		if (node[i].GetSymbol() == character) {
			printf("%d", stoi(*huffman_code[i]));
			break;
		}
	}

	for (int i = 0; i < n; i++) {
		delete[] huffman_code[i];
	}

	delete[] huffman_code;
	*/
	delete[] node;
	// fclose(inputFile);
	return 0;
}