// 2023 알고리즘(박병준 교수님)
// 과제명 : Huffman 코드를 이용한 파일압축(Compression) 프로그램 구현 및 실험
// 2020203066 김정언

#include "class.h"
using namespace std;

int main(void) {
	FILE* inputFile = fopen("test.txt", "r");	// 입력 파일 읽기 모드로 염
	int enterkey = -1;	// enterkey 개수를 세는 변수

	// 파일이 없을 경우 프로그램 종료
	if (inputFile == NULL) {
		printf("파일이 없습니다. 프로그램을 종료합니다.\n");
		return 0;
	}

	// 파일을 한 줄씩 읽어옴
	while (1) {
		char arr[MAX_LENGTH];
		char* ptr = fgets(arr, MAX_LENGTH, inputFile);

		// 파일 끝
		if (ptr == NULL) {
			break;
		}

		enterkey++;
		frequency(arr, enterkey);
	}
	
	CountSymbol();

	Node* node = new Node[Symbol_Num];	// node 배열 생성
	Node* parent_node = new Node[Symbol_Num];	// parent_node 배열 생성
	int n = 0;	// node 배열에 하나씩 저장하기 위한 변수
	int parent_num = 0;	// 부모 node 배열의 위치를 알려주는 변수
	stack<int> huffman;	// 허프만 코드를 만들기 위한 stack

	// 노드 생성
	for (int i = 0; i < 69; i++) {
		int tmp = 0;

		for (int j = 0; j < 69; j++) {
			if (Symbol_frequency[j] == 0) {	// 한번도 나오지 않은 문자일 때
				continue;
			}

			if (Symbol_frequency[tmp] <= Symbol_frequency[j]) { // 가장 빈도수 큰 위치를 찾음
				tmp = j;
			}
		}

		if (Symbol_frequency[tmp] != 0) {	// 노드 생성
			node[n].SetNode(Symbol[tmp], Symbol_frequency[tmp], 0, 0, 0);
			Node_queue.push(&node[n]);	// list에 내림차순으로 노드 저장
			Symbol_frequency[tmp] = 0;
			n++;
		}
	}

	// 생성된 노드 출력
	// print_Node();

	// 허프만 코드에 따른 트리 생성
	while (!Node_queue.empty()) {
		Node* node1 = 0;
		Node* node2 = 0;

		Ascending();	// node 큐 오름차순으로 정렬

		// 빈도수가 가장 작은 2개 선택
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

		// 노드 2개를 함침
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

	// 트리 출력
	// ShowTree(node, parent_node, parent_num, n);
	
	string** huffman_code = new string*[n];	// 허프만 코드를 저장할 배열 생성
	for (int i = 0; i < n; i++) {
		huffman_code[i] = new string[1000];
	}

	Node_Huffman(node, parent_node, parent_num, n, huffman_code, huffman);

	// 빈도수 내림차순으로 허프만 코드 출력
	//for (int i = 0; i < n; i++) {
	//	printf("%d\n", stoi(*huffman_code[i]));
	//}

	for (int i = 0; i < n; i++) {
		delete[] huffman_code[i];
	}
	delete[] huffman_code;
	delete[] node;
	
	fclose(inputFile);
	return 0;
}