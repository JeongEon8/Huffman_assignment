#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//허프만 트리 노드
typedef struct huffNode
{
	short value; //값
	int freq; //빈도
	struct huffNode* left; //왼쪽 노드
	struct huffNode* right; //오른쪽 노드
}huffNode;

////////////MIN-Heap 처리//////////////

//함수 전처리
void heap_AddNodeDirect(huffNode** head, huffNode* node);

//힙의 크기를 입력합니다.
int heapCnt = 0;

//Min-Heapify, freq값을 기준으로 정렬
//heap:힙, i:정렬을 시작할 노드의 번지
void heap_MinHeapify(huffNode** head, int i)
{
	//i의 왼쪽 노드 번호와 오른쪽 노드 번호를 지정합니다.
	int l = i * 2;
	int r = i * 2 + 1;

	//최대값 저장
	int smallest;

	//왼쪽 확인
	if (l <= heapCnt && head[l - 1]->freq < head[i - 1]->freq)
		smallest = l;
	else
		smallest = i;
	//오른쪽 확인
	if (r <= heapCnt && head[r - 1]->freq < head[smallest - 1]->freq)
		smallest = r;
	//교환
	if (smallest != i)
	{
		//Value교체
		huffNode* ntemp = head[smallest - 1];
		head[smallest - 1] = head[i - 1];
		head[i - 1] = ntemp;

		heap_MinHeapify(head, smallest);
	}
}

//힙에 값을 추가합니다.
//huffNode:힙의 헤드, val:값, freq:빈도
void heap_AddNode(huffNode** head, int val, int freq)
{
	//새로운 huffNode를 정의합니다.
	huffNode* temphn = (huffNode*)malloc(sizeof(huffNode));
	temphn->value = val;
	temphn->freq = freq;
	temphn->left = NULL;
	temphn->right = NULL;

	//새로운 노드를 추가합니다.
	heap_AddNodeDirect(head, temphn);
}

//힙에 노드를 직접 추가합니다.
void heap_AddNodeDirect(huffNode** head, huffNode* node)
{
	//새로운 노드를 추가합니다.
	head[heapCnt++] = node;

	//새로 추가된 노드를 상위 노드와 비교하며 필요한 경우 값을 교체합니다.
	int temp = heapCnt;
	while (temp > 1)
	{
		//상위 노드의 값이 더 클 경우 노드를 교체합니다.
		if (head[temp / 2 - 1]->freq > head[temp - 1]->freq)
		{
			huffNode* ntemp = head[temp / 2 - 1];
			head[temp / 2 - 1] = head[temp - 1];
			head[temp - 1] = ntemp;
		}
		else //상위 노드보다 값이 작은 경우 루프를 종료합니다.
			break;

		//조사할 노드를 지정합니다
		temp /= 2;
	}
}

//힙에서 최소값을 가져옵니다.
huffNode* heap_Pop(huffNode** head)
{
	//반환할 새로운 huffNode를 정의합니다.
	huffNode* newhn = (huffNode*)malloc(sizeof(huffNode));
	newhn->value = head[0]->value;
	newhn->freq = head[0]->freq; //최상위 노드가 최소값이다.
	newhn->left = head[0]->left;
	newhn->right = head[0]->right;

	//2개 이상의 노드가 남아있을 때
	if (heapCnt > 1)
	{
		//마지막 노드의 값을 처음 노드로 복사한 후 삭제
		head[0]->value = head[heapCnt - 1]->value;
		head[0]->freq = head[heapCnt - 1]->freq;
		head[0]->left = head[heapCnt - 1]->left;
		head[0]->right = head[heapCnt - 1]->right;
		free(head[heapCnt - 1]);
		head[--heapCnt] = NULL; //힙의 크기 줄이고 널 값 입력

		//MinHeapify 수행
		heap_MinHeapify(head, 1);
	}
	//1개 이하의 노드가 남아있을 때
	else
	{
		free(head[0]);
		head[0] = NULL;
		heapCnt--; //힙 크기 줄이기
	}

	return newhn;
}

///////////////MIN-Heap 처리 끝///////////////

////////////// Huffman Tree 처리 //////////////

//허프만 트리의 각 잎을 방문하면서 트리를 파일로 기록합니다.
//node: 방문을 시작할 노드, position: 현재 노드의 주소, level: 현재 노드의 층(head가 0), huf: 허프만 데이터를 담을 파일
//head의 주소는 '0'입니다.
void tree_VisitLeaf(huffNode* node,
	unsigned long int position, int level, FILE* huf)
{
	//허프만 특성상 한쪽만 NULL인 노드는 생기지 않는다. 따라서 한 쪽만 NULL이어도 그것은 Leaf 노드이다.
	//Leaf노드인 경우
	if (node->left == NULL)
	{
		fprintf(huf, "%d:", node->value);
		for (int i = level - 1; i >= 0; i--)
			fprintf(huf, "%d", (position >> i) & 1);
		fprintf(huf, "\n");
	}
	//Leaf노드가 아닌 경우 탐색을 계속합니다. (허프만 특성상 Leaf노드의 값만 알면 되므로, 전위, 후위 탐색으로 구분하기 힘듭니다.)
	else
	{
		tree_VisitLeaf(node->left, position << 1, level + 1, huf);
		tree_VisitLeaf(node->right, (position << 1) + 1, level + 1, huf);
	}
}

////////////// Huffman Tree 처리 끝/////////////

////////////// bit 처리 ///////////////////

//기록하고 남은 데이터가 저장되는 공간
int wbdata = 0;
//남은 데이터가 몇 비트인지 저장하는 변수
int remBit = 0;

//Bit단위로 파일에 기록합니다.
//data: 자료, len: 길이, out: 출력할 파일
void writeBit(int data, int len, FILE* out)
{
	//기존 자료와 합칩니다.
	wbdata = (wbdata << len) + data;
	remBit += len;

	while (remBit > 7)
	{
		//버퍼
		char temp[1] = { 0 };
		temp[0] = wbdata >> (remBit -= 8);
		//파일에 기록합니다.
		fwrite(temp, sizeof(char), 1, out);
	}
}

//기록을 마칩니다.
void writeFin(FILE* out)
{
	if (remBit > 0)
	{
		//버퍼
		char temp[1] = { 0 };
		temp[0] = wbdata;
		temp[0] = temp[0] << (8 - remBit); //남은 부분을 상위 비트로 올려서 기록합니다.
		//파일에 기록합니다.
		fwrite(temp, sizeof(char), 1, out);
	}
}

//해당 data를 코드를 기록합니다.
//head: 탐색할 노드, position: 현재 노드의 주소, level: 현재 노드 레벨, data: 기록할 데이타, out: 기록할 파일
void writeCode(huffNode* node, unsigned long int position,
	int level, short data, FILE* out)
{
	//허프만 특성상 한쪽만 NULL인 노드는 생기지 않는다. 따라서 한 쪽만 NULL이어도 그것은 Leaf 노드이다.
	//Leaf노드인 경우
	if (node->left == NULL)
	{
		if (data == node->value)
			for (int i = level - 1; i >= 0; i--)
				writeBit((position >> i) & 1, 1, out);
	}
	//Leaf노드가 아닌 경우 탐색을 계속합니다. (허프만 특성상 Leaf노드의 값만 알면 되므로, 전위, 후위 탐색으로 구분하기 힘듭니다.)
	else
	{
		writeCode(node->left, position << 1, level + 1, data, out);
		writeCode(node->right, (position << 1) + 1, level + 1, data, out);
	}
}

///////////// bit 처리 끝////////////////