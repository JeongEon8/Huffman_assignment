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

	fclose(inputFile);
	CountSymbol();

	Node* node = new Node[Symbol_Num];	// node 배열 생성
	int n = 0;	// node 배열에 하나씩 저장하기 위한 변수

	for (int i = 0; i < 69; i++) {
		int tmp = 0;

		for (int j = 0; j < 69; j++) {
			if (Symbol_frequency[j] == 0) {	// 한번도 나오지 않은 문자일 때
				continue;
			}

			if (Symbol_frequency[tmp] < Symbol_frequency[i]) { // 가장 빈도수 큰 위치를 찾음
				tmp = j;
			}
		}

		if (tmp != 0) {	// 노드 생성
			node[n].SetNode(Symbol[tmp], Symbol_frequency[tmp], 0, 0);
		}
	}

	delete[] node;
	return 0;
}