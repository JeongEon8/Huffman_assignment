// Compressor.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "huffman.h" //허프만 코드 관련 헤더

int main()
{
	//변수 선언
	char fileName[100];
	FILE* inputFile; //소스 파일
	FILE* outputFile; //목적 파일
	FILE* hufFile; //허프만 데이터 파일
	unsigned char buff[1]; //버퍼, 한 문자열 씩
	long int chCnt[256] = { 0, }; //0~255까지 문자열 저장

	//파일 변수 정의하기
	printf("압축할 파일 이름을 입력하세요:");
	scanf("%s", &fileName);
	inputFile = fopen(fileName, "rb");
	//입력파일 존재 여부 확인
	if (inputFile == NULL)
	{
		printf("파일을 열 수 없습니다. 프로그램을 종료합니다.\n");
		system("pause");
		return -1;
	}
	outputFile = fopen(strcat(fileName, ".hfb"), "wb");
	//hufFile명 생성
	int pos = 0;
	while (fileName[pos] != '\0')
		pos++;
	fileName[pos - 3] = 'h';
	fileName[pos - 2] = 'f';
	fileName[pos - 1] = 't';
	hufFile = fopen(strcat(fileName, ".hft"), "w");
	//출력파일 확인
	if (outputFile == NULL || hufFile == NULL)
	{
		printf("파일을 열 수 없습니다. 프로그램을 종료합니다.\n");
		system("pause");
		return -1;
	}

	//8비트(1char)씩 끊어서 빈도 측정
	while (fread(buff, sizeof(char), 1, inputFile))
	{
		chCnt[buff[0]]++;
	}

	//측정된 빈도수를 바탕으로 힙 생성
	huffNode** head = (huffNode**)malloc(sizeof(huffNode*) * 257);
	for (int i = 0; i < 256; i++)
	{
		//빈도수가 하나도 없으면 트리에 넣지 않습니다.
		if (chCnt[i] != 0)
			heap_AddNode(head, i, chCnt[i]);
	}
	heap_AddNode(head, 256, 1); //파일의 끝을 알리는 문자 추가

	//작성된 힙을 바탕으로 허프만 트리 작성
	//최종적으로 작성된 트리는 head[0]에 위치합니다. 
	while (heapCnt != 1) //Head의 크기가 하나가 될 때까지 반복합니다.
	{
		huffNode* hn = (huffNode*)malloc(sizeof(huffNode));
		hn->value = -1;
		hn->left = heap_Pop(head); //왼쪽부터 순차적으로 값을 빼냅니다.
		hn->right = heap_Pop(head);
		hn->freq = hn->left->freq + hn->right->freq; //왼쪽, 오른쪽의 빈도값을 더해서 빈도 값으로 넣습니다.

		heap_AddNodeDirect(head, hn);
	}

	//만들어진 허프만 트리를 기반으로 압축파일 생성
	rewind(inputFile); //파일의 포인터를 처음으로 이동시킵니다.
	while (fread(buff, sizeof(char), 1, inputFile))
	{
		writeCode(head[0], 0, 0, buff[0], outputFile);
	}
	writeCode(head[0], 0, 0, 256, outputFile); //파일의 끝을 기록합니다.
	writeFin(outputFile); //버퍼에 남은 정보를 모두 기록합니다.

	//트리를 파일에 기록합니다.
	tree_VisitLeaf(head[0], 0, 0, hufFile);

	//파일 닫기
	fclose(inputFile);
	fclose(outputFile);
	fclose(hufFile);

	//메세지 출력
	printf("압축을 완료했습니다.\n");
	system("pause");
	return 0;
}