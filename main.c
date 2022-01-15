#include <stdlib.h>
#include <stdio.h>

typedef struct innernode
{
	char letter;
	struct innernode* next;
} InnerNode;

typedef struct node
{
	InnerNode* word;
	struct node* next;
} Node;

InnerNode* createInnerNode(char letter)
{
	InnerNode* newNode = (InnerNode*)malloc( sizeof(InnerNode) );
	newNode->letter = letter;
	newNode->next = NULL;
	return newNode;
}

Node* createNode(InnerNode* word)
{
	Node* newNode = (Node*)malloc( sizeof(Node) );
	newNode->word = word;
	newNode->next = NULL;
	return newNode;
}

InnerNode* createWord(char* string)
{
	InnerNode* word = NULL;
	
	if(string!=NULL && string[0]!='\0')
	{
		word = createInnerNode(string[0]);
		InnerNode* current = word;
		for(int i=1; string[i]!='\0'; i++)
		{
			current->next = createInnerNode(string[i]);
			current = current->next;
		}
	}
	
	return word;
}

void deleteWord(InnerNode* word)
{
	while(word!=NULL)
	{
		InnerNode* next = word->next;
		free(word);
		word = next;
	}
}

void deleteNode(Node* node)
{
	if (node != NULL)
	{
		// delete word
		deleteWord(node->word);
		
		// free node
		free(node);
	}
}

int isEmpty(Node* head)
{
	return head == NULL ? 1 : 0;
}

void printWord(InnerNode* word)
{
	for(InnerNode* current = word; current != NULL; current = current->next)
	{
		printf("%c", current->letter);
	}
}

void printList(Node* head)
{
	printf("\n");
	
	if( isEmpty(head) )
	{
		printf("empty list");
	}
	else
	{
		for(Node* current = head; current != NULL; current = current->next)
		{
			printWord(current->word);
			printf(" ");
		}
	}
}

int wordLength(InnerNode* word)
{
	int out;
	
	for(out=0; word!=NULL; out++)
		word = word->next;
	
	return out;
}

Node* insert(Node* head, int index, InnerNode* word)
{
	// create new Node with word
	Node* newNode = createNode(word);
	
	// insert newNode into list at index
	if ( isEmpty(head) || index==0 )
	{
		newNode->next = head;
		head = newNode;
	}
	else
	{
		// find node to insert newNode behind
		Node* current = head;
		
		for(int i=0; i<index-1 && current->next!=NULL; i++)
			current = current->next;
	
		// insert newNode behind current
		newNode->next = current->next;
		current->next = newNode;
	}
	
	return head;
}

Node* append(Node* head, InnerNode* word)
{
	// create new Node with word
	Node* newNode = createNode(word);
	
	if( isEmpty(head) )
		head = newNode;
	else
	{
		// find last Node in list
		Node* tail;
		for(tail = head; tail->next != NULL; tail = tail->next);
		
		// insert newNode behind current
		tail->next = newNode;
	}
	
	return head;
}

Node* delete(Node* head, int index)
{
	if(index < 0)
		index *= -1;
	
	if ( !isEmpty(head) )
	{	
		// find node to delete
		Node* deleteMe = head;
		
		if(index == 0)
		{
			head = head->next;
			deleteNode(deleteMe);
		}
		else
		{
			Node* previousNode;
			int i;
			
			for(i=0; i<index && deleteMe->next!=NULL; i++)
			{
				previousNode = deleteMe;
				deleteMe = deleteMe->next;
			}
			
			if(i == index)
			// delete only if index actually exists in this list!
			{
				previousNode->next = deleteMe->next;
				deleteNode(deleteMe);
			}
		}
	}
	
	return head;
}

Node* deleteList(Node* head)
{
	while( !isEmpty(head) )
		head = delete(head,0);
	
	return head;
}

int compare_alphabetically(InnerNode* m, InnerNode* n)
{
	// assign ASCII value of the letter, or 0 if node pointer is NULL
	int valueM = m ? m->letter : 0;
	int valueN = n ? n->letter : 0;
	
	int diff = valueM - valueN; // indicates which is the higher value
	
	if (diff == 0) // on equal value
	{
		if( m!=NULL && n!=NULL ) // if both lists have another entry
			// continue recursively
			diff = compare_alphabetically(m->next, n->next);
	}
	
	return diff;
}

int compare_length(InnerNode* m, InnerNode* n)
{
	// calculate length for each word
	int lengthM = wordLength(m);
	int lengthN = wordLength(n);
	
	int diff = lengthM - lengthN;
	
	return diff;
}

void swap(Node* m, Node* n)
{
	// swap words (InnerNodes) only! not the Nodes!
	
	InnerNode* buffer = m->word;
	
	m->word = n->word;
	n->word = buffer;
}

Node* sort(Node* head, int (*compare)(InnerNode*, InnerNode*), int descending)
{
	// check if list contains at least two items
	if(head!=NULL && head->next!=NULL)
	{
		// sorting algorithm: bubble sort
		int sorted = 0;
		
		while(!sorted)
		{
			sorted = 1;
			// iterate over pairs of list items
			Node* current = head;
			for(; current->next!=NULL; current=current->next)
			{
				int cmp = compare(current->word, current->next->word);
				
				if(cmp>0 && !descending)
				{
					swap(current, current->next);
					sorted = 0;
				}
				else if (cmp<0 && descending)
				{
					swap(current, current->next);
					sorted = 0;
				}
			}
		}
	}
	
	return head;
}

int isValidMenuItem(char c)
{
	int out;
	
	switch(c)
	{
		case 'a': ;
		case 'i': ;
		case 'd': ;
		case 's': ;
		case 'p': ;
		case 'x': 
			out = 1;
			break;
		default:
			out = 0;
			break;
	}
	
	return out;
}

char menu(void)
{
	char input;
	
	do
	{
		printf("\nChoose action: (a)ppend, (i)nsert, (d)elete, (s)ort, (p)rint, e(x)it: ");
		scanf(" %c", &input);
	}
	while( !isValidMenuItem(input) );
	
	return input;
}

int main(void)
{
	// initialize an empty list
	Node* head = NULL;
	
	char menuItem, word[21], alpha_or_len, asc_or_desc;
	int index;
	
	do
	{
		// poll menu item from user
		menuItem = menu();
		
		switch(menuItem)
		{
			case 'a': // append word to list
				printf("\nEnter Word: ");
				scanf("%21s", &word[0]);
				head = append(head, createWord(word));
				break;
			
			case 'i': // insert word into list
				printf("\nEnter Index: ");
				scanf("%i", &index);
				printf("\nEnter Word: ");
				scanf("%21s", &word[0]);
				head = insert(head, index, createWord(word));
				break;
			
			case 'd': // delete word from list
				printf("\nEnter Index: ");
				scanf("%i", &index);
				head = delete(head, index);
				break;
			
			case 's': // sort list
				do
				{
					printf("\nSort (a)lphabetically or by (l)ength? ");
					scanf(" %c", &alpha_or_len);
				}
				while(alpha_or_len!='a' && alpha_or_len!='l');
				do
				{
					printf("\nSort (a)scending or (d)escending? ");
					scanf(" %c", &asc_or_desc);
				}
				while(asc_or_desc!='a' && asc_or_desc!='d');
				sort(head,
					  alpha_or_len == 'a' ? compare_alphabetically : compare_length,
					  asc_or_desc  == 'a' ? 0 : 1
					 );
				break;
			
			case 'p': // print list
				printList(head);
				break;
			
			case 'x': // exit program
				/* Do nothing here, as while loop will end
				 * with this 'x' input and program will then
				 * enter into its exit sequence by itself.
				 */
				break;
			
			default:
				// Do nothing here.
				break;
		}
	}
	while(menuItem != 'x');
	
	head = deleteList(head);
	
	return 0;
}
