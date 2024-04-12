#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HTSIZE 14

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
	struct RecordType* next;
}; 

// Fill out this structure
struct HashType
{
	struct RecordType** records;
};

// Compute the hash function
int hash(int x)
{
	return (x >> 3) % HTSIZE;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

void printHash(struct RecordType* record) {
	if (record != NULL) {
		printf("%d %c %d", record->id, record->name, record->order);
		if (record->next != NULL) printf(" -> ");
	}
	if (record->next != NULL) printHash(record->next);
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
		printf("%d: ", i);
		printHash(pHashArray->records[i]);
		printf("\n");
	}
}

void copyInfo(struct RecordType** copytoRecord, struct RecordType copyfromRecord) {
	if ((*copytoRecord) == NULL) {
		(*copytoRecord) = (struct RecordType*)malloc(sizeof(struct RecordType));
		(*copytoRecord)->id = copyfromRecord.id;
		(*copytoRecord)->name = copyfromRecord.name;
		(*copytoRecord)->order = copyfromRecord.order;
		(*copytoRecord)->next = NULL;
	}
	else copyInfo(&((*copytoRecord)->next), copyfromRecord);
}

void freemem(struct RecordType* record) {
	if (record->next != NULL) freemem(record->next);
	free(record);
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;
	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);

	struct HashType* myHT = (struct HashType*)malloc(sizeof(struct HashType));
	myHT->records = (struct RecordType**)malloc(sizeof(struct RecordType*) * recordSz);
	for (int i = 0; i < recordSz; i++) {
		myHT->records[i] = NULL;
	}
	for (int i = 0; i < recordSz; i++) {
		int temphash = hash(pRecords[i].id);
		copyInfo(&(myHT->records[temphash]), pRecords[i]);
	}
	
	displayRecordsInHash(myHT, HTSIZE);
	
	for (int i = 0; i < HTSIZE; i++) {
		freemem(myHT->records[i]);
	}

	free(myHT->records);
	free(myHT);

}