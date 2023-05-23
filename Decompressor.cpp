// DeCompressor.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///////////////////////////////////////////////������ Ʈ�� ���� ////////////////////////////////////

//������ Ʈ�� ���
typedef struct huffNode
{
	short value; //��
	int freq; //��
	struct huffNode* left; //���� ���
	struct huffNode* right; //������ ���
}huffNode;

//������ �о�鼭 Ʈ���� ����ϴ�.
void tree_loadHuff(huffNode* head, FILE* hufFile)
{
	//�Է��� �����͸� ���� ����
	int data;

	//������ �����鼭 Ʈ���� �����ϴ�.
	while (fscanf(hufFile, "%d:", &data) != EOF)
	{
		//node�� head�� �����ϴ�.
		huffNode* node = head;

		while (1)
		{
			int i = fgetc(hufFile);

			//�� �� ����
			if (i == '\n')
			{
				//���� ��ġ�� ��忡 ���� �ְ� �����մϴ�.
				node->value = data;
				break;
			}
			//���ڰ� ���� ��� �ش� ���� �̵��մϴ�.
			else
			{
				//���� ����� ���
				if (i == 48) //�ƽ�Ű�ڵ忡�� '0'�� 48�̹Ƿ�, ���̳ʸ��� ���� �ʱ� ������ �ƽ�Ű �ڵ带 ���
				{
					//���� ��尡 ���� ���, ���� ��带 �����մϴ�.
					if (node->left == NULL)
					{
						node->left = (huffNode*)malloc(sizeof(huffNode));
						node->left->left = NULL;
						node->left->right = NULL;
					}
					node = node->left;
				}
				//������ ����� ���
				else
				{
					//������ ��尡 ���� ���, ������ ��带 �����մϴ�.
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

////////////////////////////////////������ Ʈ�� ���� ��///////////////////////////////////////////

////////////////////////////////////////////////// bit ó�� //////////////////////////////////////////////////////////

//����ϰ� ���� �����Ͱ� ����Ǵ� ����
unsigned int wbData = 0;
//���� �����Ͱ� �� ��Ʈ���� �����ϴ� ����
int remBit = 0;

//������ �����͸� �������� ���Ͽ� ����մϴ�.
//head: �������ڷ�, in: �Է� ���� , out: ����� ����
void writeData(huffNode* head, FILE* in, FILE* out)
{
	//char ����
	//unsigned�� ���� ������ �� ó�� ��Ʈ�� 1�� �� ��� ������ ����ϹǷ� unsigned�� ������ �־�� �Ѵ�.
	unsigned char ch[1];
	//node�� head�� �����մϴ�.
	huffNode* node = head;

	while (fread(ch, sizeof(char), 1, in))
	{
		//���� �ڷ�� ��Ĩ�ϴ�.
		wbData = (wbData << 8) + ch[0];
		remBit += 8;

		//node�� ���� �ٽ� �����մϴ�.
		node = head;

		//���� ��Ʈ�� 1�� �̻��� �� �۵��մϴ�.
		for (int i = remBit - 1; i >= 0; i--)
		{
			//��带 ��Ʈ�� ���� �̵���ŵ�ϴ�.
			if (((wbData >> i) & 1) == 0)
				node = node->left;
			else
				node = node->right;

			if (node->left == NULL) //���� ��尡 Leaf�� ���
			{
				//���� ���ڰ� ���� ��� �Լ��� �����մϴ�.
				if (node->value == 256)
					return;

				//����
				char temp[1] = { node->value };
				//���Ͽ� ����մϴ�.
				fwrite(temp, sizeof(char), 1, out);
				//���� ��Ʈ���� i���� �ֽ��ϴ�.
				remBit = i;

				//node�� ���� �ٽ� �����մϴ�.
				node = head;
			}
		}
	}
}

////////////////////////////////////////////////// bit ó�� ��////////////////////////////////////////////////////////

int main()
{
	char fileName[100];
	FILE* inputFile; //�ҽ�����(����)
	FILE* outputFile; //��������(���� ������ ����)
	FILE* hufFile; //������ ������ ����

	//���� ���� �����ϱ�
	printf("������ ������ ���� �̸��� �Է��ϼ���(��:abc.txt.hfb):");
	scanf("%s", &fileName);
	inputFile = fopen(fileName, "rb");
	//�Է����� ���� ���� Ȯ��
	if (inputFile == NULL)
	{
		printf("������ �� �� �����ϴ�. ���α׷��� �����մϴ�.\n");
		system("pause");
		return -1;
	}
	outputFile = fopen(strcat(fileName, ".txt"), "wb");
	//hufFile�� ����
	int pos = 0;
	while (fileName[pos] != '\0')
		pos++;
	fileName[pos - 7] = 'h';
	fileName[pos - 6] = 'f';
	fileName[pos - 5] = 't';
	fileName[pos - 4] = '\0';
	hufFile = fopen(strcat(fileName, ".txt"), "r");
	//������� ��� ���� ���� Ȯ��
	if (outputFile == NULL || hufFile == NULL)
	{
		printf("������ �� �� �����ϴ�. ���α׷��� �����մϴ�.\n");
		system("pause");
		return -1;
	}

	//Ʈ���� ������ ����Դϴ�.
	huffNode* huff = (huffNode*)malloc(sizeof(huffNode));
	huff->right = NULL;
	huff->left = NULL;
	//Ʈ���� �����մϴ�.
	tree_loadHuff(huff, hufFile);

	//������ Ʈ���� �������� ���� ������ ������ ���ϴ�.
	writeData(huff, inputFile, outputFile);

	//�޼��� ���
	printf("���� ������ �Ϸ��߽��ϴ�.\n");
	system("pause");
	return 0;
}