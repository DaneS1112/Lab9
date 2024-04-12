#include <stdio.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order;
};

// Fill out this structure
struct HashType
{
	struct RecordType* record;
	struct HashType* next;
};

// Compute the hash function
int hash(int x)
{
	int ind = x%31;
	return ind;
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

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		if(pHashArray[i].record!=NULL){
			printf("%d %c %d\n",pHashArray[i].record->id,pHashArray[i].record->name,pHashArray[i].record->order);
		}

	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation

	int hashSz=31;

	struct HashType* hArray = (struct HashType*)malloc(hashSz * sizeof(struct HashType));

	if (hArray==NULL){
		printf("Failure of allocation");
		return 1;
	}

	for(int i=0;i<hashSz;i++){
		hArray[i].record=NULL;
		hArray[i].next=NULL;
	}

	for (int i=0;i<recordSz;i++){
		int ind = hash(pRecords[i].id);

		struct RecordType* newEntry = (struct RecordType*)malloc(sizeof(struct RecordType));
		if (hArray[ind].record==NULL){
			hArray[ind].record=&pRecords[i];
		}

		else{
			struct HashType*curr=&hArray[ind];
			while(curr->next!=NULL){
				curr=curr->next;
			}

			curr->next=(struct HashType*)malloc(sizeof(struct HashType));
			if(curr->next==NULL){
				printf("Failure of allocation");
				return 1;
			}
			curr->next->record=&pRecords[i];
			curr->next->next=NULL;

		}

	}
	displayRecordsInHash(hArray,hashSz);
	free(pRecords);

	for(int i=0;i<hashSz;i++){
		struct HashType* curr =hArray[i].next;
		while(curr!=NULL){
			struct HashType* temp=curr;
			curr=curr->next;
			free(temp);
		}
	}

	free(hArray);

	return 0;
}