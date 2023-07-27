#include <stdio.h>
#include <stdlib.h> // malloc, rand
#include <time.h> // time

#define MAX_ELEM	20

typedef struct
{
	int *heapArr;
	int	last;
	int	capacity;
} HEAP;

/* Allocates memory for heap and returns address of heap head structure
if memory overflow, NULL returned
*/
HEAP *heapCreate( int capacity);

/* Free memory for heap
*/
void heapDestroy( HEAP *heap);

/* Inserts data into heap
return 1 if successful; 0 if heap full
*/
int heapInsert( HEAP *heap, int data);

/* Reestablishes heap by moving data in child up to correct location heap array
*/
static void _reheapUp( HEAP *heap, int index);

/* Deletes root of heap and passes data back to caller
return 1 if successful; 0 if heap empty
*/
int heapDelete( HEAP *heap, int* data);

/* Reestablishes heap by moving data in root down to its correct location in the heap
*/
static void _reheapDown( HEAP *heap, int index);

/* Print heap array */
void heapPrint( HEAP *heap);

int main(void)
{
	HEAP *heap;
	int data;
	int i;
	
	heap = heapCreate(MAX_ELEM);
	
	srand( time(NULL));
	
	for (i = 0; i < MAX_ELEM; i++)
	{
		data = rand() % MAX_ELEM * 3 + 1; // 1 ~ MAX_ELEM*3 random number
				
		fprintf( stdout, "Inserting %d: ", data);
		
		// insert function call
		heapInsert( heap, data);
		
		heapPrint( heap);
 	}

	while (heap->last >= 0)
	{
		// delete function call
		heapDelete( heap, &data);

		printf( "Deleting %d: ", data);

		heapPrint( heap);
 	}
	
	heapDestroy( heap);
	
	return 0;
}
HEAP *heapCreate( int capacity){
	
	HEAP *heap = (HEAP *)malloc(sizeof(HEAP));
	
	if(!heap)	return NULL;		//overflow
	
	heap->heapArr = (int *)malloc(capacity * sizeof(int));
	heap->last = -1;
	heap->capacity = 20;
	return heap;
	
}
void heapDestroy( HEAP *heap){
	free(heap->heapArr);
	heap->last = 0;
	heap->capacity = 0;
	free(heap);
}
static void _reheapUp( HEAP *heap, int index){
	
	int pindex = index/2;	//key of parent
	int pkey = heap->heapArr[pindex];
	if(index >= 0){
		if(heap->heapArr[index]>pkey){
			heap->heapArr[pindex] = heap->heapArr[index];
			heap->heapArr[index] = pkey;
			_reheapUp(heap,pindex);
		}
	}
	
}
int heapInsert( HEAP *heap, int data){
	
	if(heap->last>=heap->capacity-1)	return 0;	//start index 0 ~ capacity-1 so max of last is capacity-1
	
	heap->last++;
	heap->heapArr[heap->last] = data;
	
	if(heap->last == 0) return 1;
	
	_reheapUp(heap,heap->last);
	return 1;
	
}
static void _reheapDown( HEAP *heap, int index){
	int left_index = 2*index + 1;
	int right_index = 2*index + 2;
	
	if(left_index<=heap->last){
		
		int left_subtree_key = heap->heapArr[left_index];
		int right_subtree_key;
		if(right_index<=heap->last)
		right_subtree_key = heap->heapArr[right_index];
		else
			right_subtree_key = -1;
		
		int large_index = 0;		
		if(left_subtree_key>right_subtree_key)
			large_index = left_index;	
		else if(left_subtree_key<=right_subtree_key)
			large_index = right_index;
				
		if(heap->heapArr[index]<heap->heapArr[large_index]){
			
			int tmp_index = large_index;			
			int tmp_key = heap->heapArr[tmp_index];
			
			heap->heapArr[tmp_index] = heap->heapArr[index];
			heap->heapArr[index] = tmp_key;
			
			_reheapDown(heap, tmp_index);
		}
		
	}
}
int heapDelete( HEAP *heap, int* data){
	
	int root = 0;
	if(heap->last<0) return 0;
	
	if(heap->last == MAX_ELEM-1)	printf("\n");
		
	*data = heap->heapArr[root];
	heap->heapArr[root] = heap->heapArr[heap->last];
	heap->last--;
	
	_reheapDown(heap,0);
	
	return 1;
}
void heapPrint( HEAP *heap){
	
	for(int i=0; i<=heap->last;i++){
		
		printf("%6d",heap->heapArr[i]);
		
	}
	printf("\n");
	
}














