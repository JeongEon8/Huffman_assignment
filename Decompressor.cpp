// DeCompressor.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///////////////////////////////////////////////허프만 트리 관련 ////////////////////////////////////

//허프만 트리 노드
typedef struct huffNode
{
	short value; //값
	int freq; //빈도
	struct huffNode* left; //왼쪽 노드
	struct huffNode* right; //오른쪽 노드
}huffNode;

//파일을 읽어가면서 트리를 만듭니다.
void tree_loadHuff(huffNode* head, FILE* hufFile)
{
	//입력할 데이터를 담을 변수
	int data;

	//파일을 읽으면서 트리를 만들어갑니다.
	while (fscanf(hufFile, "%d:", &data) != EOF)
	{
		//node를 head로 돌립니다.
		huffNode* node = head;

		while (1)
		{
			int i = fgetc(hufFile);

			//한 줄 종료
			if (i == '\n')
			{
				//현재 위치한 노드에 값을 넣고 종료합니다.
				node->value = data;
				break;
			}
			//숫자가 나온 경우 해당 노드로 이동합니다.
			else
			{
				//왼쪽 노드인 경우
				if (i == 48) //아스키코드에서 '0'이 48이므로, 바이너리로 읽지 않기 때문에 아스키 코드를 사용
				{
					//왼쪽 노드가 없는 경우, 왼쪽 노드를 생성합니다.
					if (node->left == NULL)
					{
						node->left = (huffNode*)malloc(sizeof(huffNode));
						node->left->left = NULL;
						node->left->right = NULL;
					}
					node = node->left;
				}
				//오른쪽 노드인 경우
				else
				{
					//오른쪽 노드가 없는 경우, 오른쪽 노드를 생성합니다.
					if (node->right == NULL)
					{
						node->right = (huffNode*)malloc(sizeof(huffNode));
						node->right->left = NULL;
						node->right->right = NULL;
					}
					node = node->right;
				}
			}
		}
	}
}

////////////////////////////////////허프만 트리 관련 끝///////////////////////////////////////////

////////////////////////////////////////////////// bit 처리 //////////////////////////////////////////////////////////

//기록하고 남은 데이터가 저장되는 공간
unsigned int wbData = 0;
//남은 데이터가 몇 비트인지 저장하는 변수
int remBit = 0;

//허프만 데이터를 기준으로 파일에 기록합니다.
//head: 허프만자료, in: 입력 파일 , out: 출력할 파일
void writeData(huffNode* head, FILE* in, FILE* out)
{
	//char 변수
	//unsigned를 하지 않으면 맨 처음 비트가 1이 될 경우 음수로 계산하므로 unsigned로 지정해 주어야 한다.
	unsigned char ch[1];
	//node에 head를 저장합니다.
	huffNode* node = head;

	while (fread(ch, sizeof(char), 1, in))
	{
		//기존 자료와 합칩니다.
		wbData = (wbData << 8) + ch[0];
		remBit += 8;

		//node의 값을 다시 설정합니다.
		node = head;

		//남은 비트가 1개 이상일 때 작동합니다.
		for (int i = remBit - 1; i >= 0; i--)
		{
			//노드를 비트에 맞춰 이동시킵니다.
			if (((wbData >> i) & 1) == 0)
				node = node->left;
			else
				node = node->right;

			if (node->left == NULL) //현재 노드가 Leaf인 경우
			{
				//종결 문자가 나온 경우 함수를 종료합니다.
				if (node->value == 256)
					return;

				//버퍼
				char temp[1] = { node->value };
				//파일에 기록합니다.
				fwrite(temp, sizeof(char), 1, out);
				//남은 비트수에 i값을 넣습니다.
				remBit = i;

				//node의 값을 다시 설정합니다.
				node = head;
			}
		}
	}
}

////////////////////////////////////////////////// bit 처리 끝////////////////////////////////////////////////////////

int main()
{
	char fileName[100];
	FILE* inputFile; //소스파일(압축)
	FILE* outputFile; //목적파일(압축 해제된 파일)
	FILE* hufFile; //허프만 데이터 파일

	//파일 변수 정의하기
	printf("압축을 해제할 파일 이름을 입력하세요(예:abc.txt.hfb):");
	scanf("%s", &fileName);
	inputFile = fopen(fileName, "rb");
	//입력파일 존재 여부 확인
	if (inputFile == NULL)
	{
		printf("파일을 열 수 없습니다. 프로그램을 종료합니다.\n");
		system("pause");
		return -1;
	}
	outputFile = fopen(strcat(fileName, ".txt"), "wb");
	//hufFile명 생성
	int pos = 0;
	while (fileName[pos] != '\0')
		pos++;
	fileName[pos - 7] = 'h';
	fileName[pos - 6] = 'f';
	fileName[pos - 5] = 't';
	fileName[pos - 4] = '\0';
	hufFile = fopen(strcat(fileName, ".txt"), "r");
	//출력파일 사용 가능 여부 확인
	if (outputFile == NULL || hufFile == NULL)
	{
		printf("파일을 열 수 없습니다. 프로그램을 종료합니다.\n");
		system("pause");
		return -1;
	}

	//트리를 생성할 노드입니다.
	huffNode* huff = (huffNode*)malloc(sizeof(huffNode));
	huff->right = NULL;
	huff->left = NULL;
	//트리를 생성합니다.
	tree_loadHuff(huff, hufFile);

	//생성된 트리를 바탕으로 원본 파일을 생성해 냅니다.
	writeData(huff, inputFile, outputFile);

	//메세지 출력
	printf("압축 해제를 완료했습니다.\n");
	system("pause");
	return 0;
}