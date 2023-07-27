#include <stdlib.h> // malloc
#include <stdio.h>
#include <string.h> // strdup, strcmp
#include <ctype.h> // toupper

#define QUIT			1
#define FORWARD_PRINT	2
#define BACKWARD_PRINT	3
#define SEARCH			4
#define DELETE			5
#define COUNT			6

// User structure type definition
typedef struct 
{
	char	*token;
	int		freq;
} tTOKEN;

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	tTOKEN		*dataPtr;
	struct node	*llink;
	struct node	*rlink;
} NODE;

typedef struct
{
	int		count;
	NODE	*pos; // unused
	NODE	*head;
	NODE	*rear;
} LIST;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a list head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
LIST *createList( void);

/* Deletes all data in list and recycles memory
*/
void destroyList( LIST *pList);

/* Inserts data into list
	return	0 if overflow
			1 if successful
			2 if duplicated key
*/
int addNode( LIST *pList, tTOKEN *dataInPtr);

/* Removes data from list
	return	0 not found
			1 deleted
*/
int removeNode( LIST *pList, char *keyPtr, tTOKEN **dataOut);

/* interface to search function
	Argu	key being sought
	dataOut	contains found data
	return	1 successful
			0 not found
*/
int searchList( LIST *pList, char *pArgu, tTOKEN **pDataOut);

/* returns number of nodes in list
*/
int countList( LIST *pList);

/* returns	1 empty
			0 list has data
*/
int emptyList( LIST *pList);

//int fullList( LIST *pList);

/* prints data from list (forward)
*/
void printList( LIST *pList);

/* prints data from list (backward)
*/
void printListR( LIST *pList);

/* internal insert function
	inserts data into a new node
	return	1 if successful
			0 if memory overflow
*/
static int _insert( LIST *pList, NODE *pPre, tTOKEN *dataInPtr);

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tTOKEN **dataOutPtr);

/* internal search function
	searches list and passes back address of node
	containing target and its logical predecessor
	return	1 found
			0 not found
*/
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, char *pArgu);

/* Allocates dynamic memory for a token structure, initialize fields(token, freq) and returns its address to caller
	return	token structure pointer
			NULL if overflow
*/
tTOKEN *createToken( char *str);

/* Deletes all data in token structure and recycles memory
	return	NULL head pointer
*/
void destroyToken( tTOKEN *pToken);

////////////////////////////////////////////////////////////////////////////////
/* gets user's input
*/
int get_action()
{
	char ch;
	scanf( "%c", &ch);
	ch = toupper( ch);
	switch( ch)
	{
		case 'Q':
			return QUIT;
		case 'F':
			return FORWARD_PRINT;
		case 'B':
			return BACKWARD_PRINT;
		case 'S':
			return SEARCH;
		case 'D':
			return DELETE;
		case 'C':
			return COUNT;
	}
	return 0; // undefined action
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	LIST *list;
	char str[1024];
	tTOKEN *pToken;
	int ret;
	FILE *fp;
	
	if (argc != 2){
		fprintf( stderr, "usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (!fp)
	{
		fprintf( stderr, "Error: cannot open file [%s]\n", argv[1]);
		return 2;
	}
	
	// creates an empty list
	list = createList();
	if (!list)
	{
		printf( "Cannot create list\n");
		return 100;
	}
	
	while(fscanf( fp, "%s", str) == 1)
	{
		pToken = createToken( str);
		
		// insert function call
		ret = addNode( list, pToken);	//strdup(str)string 복제
		
		if (ret == 2) // duplicated 
			destroyToken( pToken);
	}
	
	fclose( fp);
	
	fprintf( stderr, "Select Q)uit, F)orward print, B)ackward print, S)earch, D)elete, C)ount: ");
	
	while (1)
	{
		int action = get_action();
		
		switch( action)
		{
			case QUIT:
				destroyList( list);
				return 0;
			
			case FORWARD_PRINT:
				printList( list);
				break;
			
			case BACKWARD_PRINT:
				printListR( list);
				break;
			
			case SEARCH:
				fprintf( stderr, "Input a string to find: ");
				fscanf( stdin, "%s", str);
				
				if (searchList( list, str, &pToken))
				{
					fprintf( stdout, "(%s, %d)\n", pToken->token, pToken->freq);
				}
				else fprintf( stdout, "%s not found\n", str);
				break;
				
			case DELETE:
				fprintf( stderr, "Input a string to delete: ");
				fscanf( stdin, "%s", str);
				
				if (removeNode( list, str, &pToken))
				{
					fprintf( stdout, "%s deleted\n", pToken->token);
					destroyToken( pToken);
				}
				else fprintf( stdout, "%s not found\n", str);
				break;
			
			case COUNT:
				fprintf( stdout, "%d\n", countList( list));
				break;
			}
		
		if (action) fprintf( stderr, "Select Q)uit, F)orward print, B)ackward print, S)earch, D)elete, C)ount: ");
	}
	return 0;
}
LIST *createList( void){
	LIST *list = (LIST *)malloc(sizeof(LIST));
	
	if(!list) return 0;
	
	list->count = 0;
	list->pos = NULL;
	list->head = NULL;
	list->rear = NULL;
	return list;
}
void destroyList( LIST *pList){
	if(pList->count == 0){
		free(pList);
		return;
	}
	NODE *node,*next;
	node = pList->head;
	while(node != NULL){
		tTOKEN *temp;
		temp=node->dataPtr;
		next = node->rlink;
		destroyToken(temp);
		free(node);
		node = next;
	}
	pList->count = 0;
	free(pList);
}
int addNode( LIST *pList, tTOKEN *dataInPtr){
	NODE *pre, *loc;
	
	if(_search(pList, &pre, &loc, dataInPtr->token)){	//already exist
		loc->dataPtr->freq++;
		return 2;
	}
	if(_insert(pList, pre, dataInPtr)==0){
		
		return 0;
	}
	pList->count++;
	return 1;

}
int removeNode( LIST *pList, char *keyPtr, tTOKEN **dataOut){
	if(pList->count == 0){
		return 0;
	}
	NODE *pre = NULL;
	NODE *loc = NULL;
	
	if(_search(pList,&pre,&loc,keyPtr) == 0)	{
		*dataOut = NULL;
		return 0;
	}
	_delete(pList,pre,loc,dataOut);
	pList->count--;
	return 1;
	
}
int searchList( LIST *pList, char *pArgu, tTOKEN **pDataOut){
	NODE *pre = NULL;
	NODE *loc = NULL;
 	if(_search(pList,&pre,&loc,pArgu))	{
		*pDataOut = loc->dataPtr;
		return 1;
	}
	return 0;	//not exist
}
int countList( LIST *pList){
	return pList->count;
}
int emptyList( LIST *pList){
	if(pList->count == 0){
		return 1;
	}
	else return 0;
}
void printList( LIST *pList){
	NODE *node;
	tTOKEN *data;
	node = pList->head;
	while(node != NULL){
		data = node->dataPtr;
		fprintf(stdout,"%s\t%d\n", data->token, data->freq);
		node = node->rlink;
	}	
}
void printListR( LIST *pList){
	NODE *node;
	tTOKEN *data;
	node = pList->rear;
	while(node != NULL){
		data = node->dataPtr;
		fprintf(stdout,"%s\t%d\n", data->token, data->freq);
		node = node->llink;
	}
}
static int _insert( LIST *pList, NODE *pPre, tTOKEN *dataInPtr){
	NODE *curr;
	curr=(NODE *)malloc(sizeof(NODE));
	
	if(curr == NULL){
		return 0;	//overflow
	}
	curr->dataPtr = dataInPtr;
	curr->llink = NULL;
	curr->rlink = NULL;
	if(pList->count == 0){
		pList->head = curr;
		pList->rear = curr;
	}
	else if(pPre == NULL){		
		NODE *first = pList->head;
		first->llink = curr;
		curr->rlink = first;
		pList->head = curr;
		
	}
	else if(pPre->rlink == NULL){		
		pPre->rlink = curr;
		curr->llink = pPre;
		pList->rear = curr;
	}
	else{
		NODE *next = pPre->rlink;
		curr->rlink = next;
		next->llink = curr;
		curr->llink = pPre;
		pPre->rlink = curr;
	}
	
	return 1;
}
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tTOKEN **dataOutPtr){
	*dataOutPtr = pLoc->dataPtr;	//to save dataOutPtr's links
	if(pPre == NULL){	
		pList->head = NULL;
	}
	else if(pLoc->rlink == NULL){
		pPre->rlink = NULL;
		pList->rear = pPre;
	}
	else{
		NODE *next = pLoc->rlink;
		next->llink = pLoc;
		pPre->rlink = next;
		next->llink = pPre;		
		free(pLoc);
	}
}
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, char *pArgu){
	if(pList->count == 0)	{
		return 0;
	}
	
	*pPre = pList->head;
	*pLoc = pList->head;

	while(*pLoc != NULL){
		tTOKEN *copy=(*pLoc)->dataPtr;
		int check = strcmp(pArgu,copy->token);
		if(*pLoc == pList->head){
				*pPre = NULL;
			}
		if(check == 0){	//pArgu exists
			return 1;
		}
		else if(check < 0){	
			return 0;
		}
		*pPre = *pLoc;
		*pLoc = (*pLoc)->rlink;
	}
	
	return 0;
}
tTOKEN *createToken( char *str){
	tTOKEN *pToken;
	pToken = (tTOKEN *)malloc(sizeof(tTOKEN));
	if(pToken == NULL)	return NULL;
	pToken->token = strdup(str);
	pToken->freq = 1;
	
	return pToken;
}
void destroyToken( tTOKEN *pToken){
	free(pToken->token);
	pToken->freq=0;
	free(pToken);
}


