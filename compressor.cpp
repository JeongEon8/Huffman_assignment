// Compressor.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "huffman.h" //������ �ڵ� ���� ���

int main()
{
	//���� ����
	char fileName[100];
	FILE* inputFile; //�ҽ� ����
	FILE* outputFile; //���� ����
	FILE* hufFile; //������ ������ ����
	unsigned char buff[1]; //����, �� ���ڿ� ��
	long int chCnt[256] = { 0, }; //0~255���� ���ڿ� ����

	//���� ���� �����ϱ�
	printf("������ ���� �̸��� �Է��ϼ���:");
	scanf("%s", &fileName);
	inputFile = fopen(fileName, "rb");
	//�Է����� ���� ���� Ȯ��
	if (inputFile == NULL)
	{
		printf("������ �� �� �����ϴ�. ���α׷��� �����մϴ�.\n");
		system("pause");
		return -1;
	}
	outputFile = fopen(strcat(fileName, ".hfb"), "wb");
	//hufFile�� ����
	int pos = 0;
	while (fileName[pos] != '\0')
		pos++;
	fileName[pos - 3] = 'h';
	fileName[pos - 2] = 'f';
	fileName[pos - 1] = 't';
	hufFile = fopen(strcat(fileName, ".hft"), "w");
	//������� Ȯ��
	if (outputFile == NULL || hufFile == NULL)
	{
		printf("������ �� �� �����ϴ�. ���α׷��� �����մϴ�.\n");
		system("pause");
		return -1;
	}

	//8��Ʈ(1char)�� ��� �� ����
	while (fread(buff, sizeof(char), 1, inputFile))
	{
		chCnt[buff[0]]++;
	}

	//������ �󵵼��� �������� �� ����
	huffNode** head = (huffNode**)malloc(sizeof(huffNode*) * 257);
	for (int i = 0; i < 256; i++)
	{
		//�󵵼��� �ϳ��� ������ Ʈ���� ���� �ʽ��ϴ�.
		if (chCnt[i] != 0)
			heap_AddNode(head, i, chCnt[i]);
	}
	heap_AddNode(head, 256, 1); //������ ���� �˸��� ���� �߰�

	//�ۼ��� ���� �������� ������ Ʈ�� �ۼ�
	//���������� �ۼ��� Ʈ���� head[0]�� ��ġ�մϴ�. 
	while (heapCnt != 1) //Head�� ũ�Ⱑ �ϳ��� �� ������ �ݺ��մϴ�.
	{
		huffNode* hn = (huffNode*)malloc(sizeof(huffNode));
		hn->value = -1;
		hn->left = heap_Pop(head); //���ʺ��� ���������� ���� �����ϴ�.
		hn->right = heap_Pop(head);
		hn->freq = hn->left->freq + hn->right->freq; //����, �������� �󵵰��� ���ؼ� �� ������ �ֽ��ϴ�.

		heap_AddNodeDirect(head, hn);
	}

	//������� ������ Ʈ���� ������� �������� ����
	rewind(inputFile); //������ �����͸� ó������ �̵���ŵ�ϴ�.
	while (fread(buff, sizeof(char), 1, inputFile))
	{
		writeCode(head[0], 0, 0, buff[0], outputFile);
	}
	writeCode(head[0], 0, 0, 256, outputFile); //������ ���� ����մϴ�.
	writeFin(outputFile); //���ۿ� ���� ������ ��� ����մϴ�.

	//Ʈ���� ���Ͽ� ����մϴ�.
	tree_VisitLeaf(head[0], 0, 0, hufFile);

	//���� �ݱ�
	fclose(inputFile);
	fclose(outputFile);
	fclose(hufFile);

	//�޼��� ���
	printf("������ �Ϸ��߽��ϴ�.\n");
	system("pause");
	return 0;
}