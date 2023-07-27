#define SHOW_STEP 0
#define BALANCING 1		//insert 함수에서 사용

#include <stdlib.h> // malloc, rand
#include <stdio.h>
#include <time.h> // time
#include <string.h> //strcmp, strdup

#define max(x, y)	(((x) > (y)) ? (x) : (y))

////////////////////////////////////////////////////////////////////////////////
// AVL_TREE type definition
typedef struct node
{
	char		*data;
	struct node	*left;
	struct node	*right;
	int			height;
} NODE;

typedef struct
{
	NODE	*root;
	int		count;  // number of nodes
} AVL_TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE *AVL_Create( void);

/* Deletes all data in tree and recycles memory
*/
void AVL_Destroy( AVL_TREE *pTree);
static void _destroy( NODE *root);

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int AVL_Insert( AVL_TREE *pTree, char *data);

/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE *_insert( NODE *root, NODE *newPtr);

static NODE *_makeNode( char *data);

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
char *AVL_Retrieve( AVL_TREE *pTree, char *key);

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, char *key);

/* Prints tree using inorder traversal
*/
void AVL_Traverse( AVL_TREE *pTree);
static void _traverse( NODE *root);

/* Prints tree using inorder right-to-left traversal
*/
void printTree( AVL_TREE *pTree);
/* internal traversal function
*/
static void _infix_print( NODE *root, int level);

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight( NODE *root);

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE *rotateRight( NODE *root);

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE *rotateLeft( NODE *root);

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	AVL_TREE *tree;
	char str[1024];
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 0;
	}
	
	// creates a null tree
	tree = AVL_Create();
	
	if (!tree)
	{
		fprintf( stderr, "Cannot create tree!\n");
		return 100;
	}

	FILE *fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "Cannot open file! [%s]\n", argv[1]);
		return 200;
	}

	while(fscanf( fp, "%s", str) != EOF)
	{

#if SHOW_STEP
		fprintf( stdout, "Insert %s>\n", str);
#endif		
		// insert function call
		AVL_Insert( tree, str);
		
#if SHOW_STEP
		fprintf( stdout, "Tree representation:\n");
		printTree( tree);
#endif
	}
	
	fclose( fp);
	
#if SHOW_STEP
	fprintf( stdout, "\n");

	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	AVL_Traverse( tree);
	fprintf( stdout, "\n");

	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
#endif
	fprintf( stdout, "Height of tree: %d\n", tree->root->height);
	fprintf( stdout, "# of nodes: %d\n", tree->count);
	
	// retrieval
	char *key;
	fprintf( stdout, "Query: ");
	while( fscanf( stdin, "%s", str) != EOF)
	{
		key = AVL_Retrieve( tree, str);
		
		if (key) fprintf( stdout, "%s found!\n", key);
		else fprintf( stdout, "%s NOT found!\n", str);
		
		fprintf( stdout, "Query: ");
	}
	
	// destroy tree
	AVL_Destroy( tree);

	return 0;
}


AVL_TREE *AVL_Create( void){
	AVL_TREE *tree = (AVL_TREE*)malloc(sizeof(AVL_TREE));
	
	if(!tree)	return NULL;	//overflow
	
	tree->root = NULL;
	tree->count = 0;
	
	return tree;
}
static void _destroy( NODE *root){
	
	if(root!=NULL){
		
		_destroy(root->left);
		_destroy(root->right);
		root->height = 0;
		free(root->data);
		free(root);
		
	}
	else{
		free(root);
	}
}
static int getHeight( NODE *root){
	
	if(root == NULL)
		return 0;
	
	return root->height;
	
}
void AVL_Destroy( AVL_TREE *pTree){
	
	if(pTree) _destroy(pTree->root);
	pTree->count = 0;
	
	free(pTree);
}
static NODE *rotateRight( NODE *root){
	
	NODE *rootL = root->left;
	NODE *newrootR = rootL->right;
	
	rootL->right = root;
	root->left = newrootR;
	
	int height_l = getHeight(root->left);
	int height_r = getHeight(root->right);
	root->height = max(height_l,height_r) + 1;
	
	height_l = getHeight(rootL->left);
	height_r = getHeight(rootL->right);
	rootL->height = max(height_l,height_r) + 1;
	
	return rootL;

}
static NODE *rotateLeft( NODE *root){
	
	NODE *rootR = root->right;
	NODE *newrootL = rootR->left;
	
	rootR->left = root;
	root->right = newrootL;
	
	int height_l = getHeight(root->left);
	int height_r = getHeight(root->right);
	root->height = max(height_l,height_r) + 1;
	
	height_l = getHeight(rootR->left);
	height_r = getHeight(rootR->right);
	rootR->height = max(height_l,height_r) + 1;
	
	return rootR;
}
static NODE *_insert( NODE *root, NODE *newPtr){
	
	if(root == NULL){
		return newPtr;
	}	
	
	int height_l, height_r;
	NODE *tmp;
	
	if(strcmp(root->data,newPtr->data) > 0){
		root->left = _insert(root->left,newPtr);
	
#if BALANCING

		height_l = getHeight(root->left);
		height_r = getHeight(root->right);
		
		root->height = max(height_l,height_r) + 1;
	
		if(height_l-height_r > 1){
		
			tmp = root->left;
	
			height_l = getHeight(tmp->left);
			height_r = getHeight(tmp->right);
	
			//leftBalance
			if(height_l-height_r < 0)
				root->left = rotateLeft(tmp);
			return rotateRight(root);
		
		}
#endif
	}
	else{
		root->right = _insert(root->right,newPtr);
#if BALANCING
	
		height_l = getHeight(root->left);
		height_r = getHeight(root->right);
		
		root->height = max(height_l,height_r) + 1;
	
		if(height_l-height_r < -1){
	
			tmp = root->right;
	
			height_l = getHeight(tmp->left);
			height_r = getHeight(tmp->right);
	
			//rightBalance
			if(height_l-height_r > 0)
				root->right = rotateRight(tmp);
			
			return rotateLeft(root);
		
		}
#endif

	}
	
	return root;
	
}
static NODE *_makeNode( char *data){
	NODE *node = (NODE*)malloc(sizeof(NODE));
	
	if(!node)	return NULL;
	
	node->data = strdup(data);
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	
	return node;
}
int AVL_Insert( AVL_TREE *pTree, char *data){
	NODE *new = _makeNode(data);
	
	if(new == NULL)	return 0;
	
	if(pTree->root == NULL){
		pTree->root = new;
		pTree->count++;
		return 1;
	}
	
	pTree->root = _insert(pTree->root,new);
	pTree->count++;
	
	return 1;
}
static NODE *_retrieve( NODE *root, char *key){
	
	if(root == NULL)	return NULL;
	
	if(strcmp(root->data,key)>0){
		root = _retrieve(root->left,key);
	}
	else if(strcmp(root->data,key)<0)
		root = _retrieve(root->right,key);
	else
		return root;
		
}
char *AVL_Retrieve( AVL_TREE *pTree, char *key){
	if(strcmp(pTree->root->data,key) == 0)	return key;
	
	NODE *node = _retrieve(pTree->root,key);
	if(node == NULL) return NULL;
	return node->data;
}
static void _traverse( NODE *root){
	if(root==NULL)	return;
	_traverse(root->left);
	printf("%s ",root->data);
	_traverse(root->right);
}
void AVL_Traverse( AVL_TREE *pTree){
	_traverse(pTree->root);
}
static void _infix_print( NODE *root, int level){
	if(root!=NULL){
		if(root->right!=NULL){
			_infix_print(root->right,level+1);
		}
		for(int i=0;i<level;i++)
			printf("\t");
		printf("%s\n",root->data);
		if(root->left!=NULL){
			_infix_print(root->left,level+1);
		}
	}
}
void printTree( AVL_TREE *pTree){
	_infix_print(pTree->root, 0);
}





