#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//������ Ʈ�� ���
typedef struct huffNode
{
	short value; //��
	int freq; //��
	struct huffNode* left; //���� ���
	struct huffNode* right; //������ ���
}huffNode;

////////////MIN-Heap ó��//////////////

//�Լ� ��ó��
void heap_AddNodeDirect(huffNode** head, huffNode* node);

//���� ũ�⸦ �Է��մϴ�.
int heapCnt = 0;

//Min-Heapify, freq���� �������� ����
//heap:��, i:������ ������ ����� ����
void heap_MinHeapify(huffNode** head, int i)
{
	//i�� ���� ��� ��ȣ�� ������ ��� ��ȣ�� �����մϴ�.
	int l = i * 2;
	int r = i * 2 + 1;

	//�ִ밪 ����
	int smallest;

	//���� Ȯ��
	if (l <= heapCnt && head[l - 1]->freq < head[i - 1]->freq)
		smallest = l;
	else
		smallest = i;
	//������ Ȯ��
	if (r <= heapCnt && head[r - 1]->freq < head[smallest - 1]->freq)
		smallest = r;
	//��ȯ
	if (smallest != i)
	{
		//Value��ü
		huffNode* ntemp = head[smallest - 1];
		head[smallest - 1] = head[i - 1];
		head[i - 1] = ntemp;

		heap_MinHeapify(head, smallest);
	}
}

//���� ���� �߰��մϴ�.
//huffNode:���� ���, val:��, freq:��
void heap_AddNode(huffNode** head, int val, int freq)
{
	//���ο� huffNode�� �����մϴ�.
	huffNode* temphn = (huffNode*)malloc(sizeof(huffNode));
	temphn->value = val;
	temphn->freq = freq;
	temphn->left = NULL;
	temphn->right = NULL;

	//���ο� ��带 �߰��մϴ�.
	heap_AddNodeDirect(head, temphn);
}

//���� ��带 ���� �߰��մϴ�.
void heap_AddNodeDirect(huffNode** head, huffNode* node)
{
	//���ο� ��带 �߰��մϴ�.
	head[heapCnt++] = node;

	//���� �߰��� ��带 ���� ���� ���ϸ� �ʿ��� ��� ���� ��ü�մϴ�.
	int temp = heapCnt;
	while (temp > 1)
	{
		//���� ����� ���� �� Ŭ ��� ��带 ��ü�մϴ�.
		if (head[temp / 2 - 1]->freq > head[temp - 1]->freq)
		{
			huffNode* ntemp = head[temp / 2 - 1];
			head[temp / 2 - 1] = head[temp - 1];
			head[temp - 1] = ntemp;
		}
		else //���� ��庸�� ���� ���� ��� ������ �����մϴ�.
			break;

		//������ ��带 �����մϴ�
		temp /= 2;
	}
}

//������ �ּҰ��� �����ɴϴ�.
huffNode* heap_Pop(huffNode** head)
{
	//��ȯ�� ���ο� huffNode�� �����մϴ�.
	huffNode* newhn = (huffNode*)malloc(sizeof(huffNode));
	newhn->value = head[0]->value;
	newhn->freq = head[0]->freq; //�ֻ��� ��尡 �ּҰ��̴�.
	newhn->left = head[0]->left;
	newhn->right = head[0]->right;

	//2�� �̻��� ��尡 �������� ��
	if (heapCnt > 1)
	{
		//������ ����� ���� ó�� ���� ������ �� ����
		head[0]->value = head[heapCnt - 1]->value;
		head[0]->freq = head[heapCnt - 1]->freq;
		head[0]->left = head[heapCnt - 1]->left;
		head[0]->right = head[heapCnt - 1]->right;
		free(head[heapCnt - 1]);
		head[--heapCnt] = NULL; //���� ũ�� ���̰� �� �� �Է�

		//MinHeapify ����
		heap_MinHeapify(head, 1);
	}
	//1�� ������ ��尡 �������� ��
	else
	{
		free(head[0]);
		head[0] = NULL;
		heapCnt--; //�� ũ�� ���̱�
	}

	return newhn;
}

///////////////MIN-Heap ó�� ��///////////////

////////////// Huffman Tree ó�� //////////////

//������ Ʈ���� �� ���� �湮�ϸ鼭 Ʈ���� ���Ϸ� ����մϴ�.
//node: �湮�� ������ ���, position: ���� ����� �ּ�, level: ���� ����� ��(head�� 0), huf: ������ �����͸� ���� ����
//head�� �ּҴ� '0'�Դϴ�.
void tree_VisitLeaf(huffNode* node,
	unsigned long int position, int level, FILE* huf)
{
	//������ Ư���� ���ʸ� NULL�� ���� ������ �ʴ´�. ���� �� �ʸ� NULL�̾ �װ��� Leaf ����̴�.
	//Leaf����� ���
	if (node->left == NULL)
	{
		fprintf(huf, "%d:", node->value);
		for (int i = level - 1; i >= 0; i--)
			fprintf(huf, "%d", (position >> i) & 1);
		fprintf(huf, "\n");
	}
	//Leaf��尡 �ƴ� ��� Ž���� ����մϴ�. (������ Ư���� Leaf����� ���� �˸� �ǹǷ�, ����, ���� Ž������ �����ϱ� ����ϴ�.)
	else
	{
		tree_VisitLeaf(node->left, position << 1, level + 1, huf);
		tree_VisitLeaf(node->right, (position << 1) + 1, level + 1, huf);
	}
}

////////////// Huffman Tree ó�� ��/////////////

////////////// bit ó�� ///////////////////

//����ϰ� ���� �����Ͱ� ����Ǵ� ����
int wbdata = 0;
//���� �����Ͱ� �� ��Ʈ���� �����ϴ� ����
int remBit = 0;

//Bit������ ���Ͽ� ����մϴ�.
//data: �ڷ�, len: ����, out: ����� ����
void writeBit(int data, int len, FILE* out)
{
	//���� �ڷ�� ��Ĩ�ϴ�.
	wbdata = (wbdata << len) + data;
	remBit += len;

	while (remBit > 7)
	{
		//����
		char temp[1] = { 0 };
		temp[0] = wbdata >> (remBit -= 8);
		//���Ͽ� ����մϴ�.
		fwrite(temp, sizeof(char), 1, out);
	}
}

//����� ��Ĩ�ϴ�.
void writeFin(FILE* out)
{
	if (remBit > 0)
	{
		//����
		char temp[1] = { 0 };
		temp[0] = wbdata;
		temp[0] = temp[0] << (8 - remBit); //���� �κ��� ���� ��Ʈ�� �÷��� ����մϴ�.
		//���Ͽ� ����մϴ�.
		fwrite(temp, sizeof(char), 1, out);
	}
}

//�ش� data�� �ڵ带 ����մϴ�.
//head: Ž���� ���, position: ���� ����� �ּ�, level: ���� ��� ����, data: ����� ����Ÿ, out: ����� ����
void writeCode(huffNode* node, unsigned long int position,
	int level, short data, FILE* out)
{
	//������ Ư���� ���ʸ� NULL�� ���� ������ �ʴ´�. ���� �� �ʸ� NULL�̾ �װ��� Leaf ����̴�.
	//Leaf����� ���
	if (node->left == NULL)
	{
		if (data == node->value)
			for (int i = level - 1; i >= 0; i--)
				writeBit((position >> i) & 1, 1, out);
	}
	//Leaf��尡 �ƴ� ��� Ž���� ����մϴ�. (������ Ư���� Leaf����� ���� �˸� �ǹǷ�, ����, ���� Ž������ �����ϱ� ����ϴ�.)
	else
	{
		writeCode(node->left, position << 1, level + 1, data, out);
		writeCode(node->right, (position << 1) + 1, level + 1, data, out);
	}
}

///////////// bit ó�� ��////////////////