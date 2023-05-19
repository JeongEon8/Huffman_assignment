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

	fclose(inputFile);
	CountSymbol();

	Node* node = new Node[Symbol_Num];	// node �迭 ����
	int n = 0;	// node �迭�� �ϳ��� �����ϱ� ���� ����

	for (int i = 0; i < 69; i++) {
		int tmp = 0;

		for (int j = 0; j < 69; j++) {
			if (Symbol_frequency[j] == 0) {	// �ѹ��� ������ ���� ������ ��
				continue;
			}

			if (Symbol_frequency[tmp] < Symbol_frequency[i]) { // ���� �󵵼� ū ��ġ�� ã��
				tmp = j;
			}
		}

		if (tmp != 0) {	// ��� ����
			node[n].SetNode(Symbol[tmp], Symbol_frequency[tmp], 0, 0);
		}
	}

	delete[] node;
	return 0;
}