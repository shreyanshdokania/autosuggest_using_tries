#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

using namespace std;

// define character size
#define CHAR_SIZE 27
#define SPACE 32

#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

// node of a trie
struct Trie
{
	int isEndOfWord;	// 1 when node is a leaf node
	struct Trie* Children[CHAR_SIZE];
};

// Returns a new Trie node
struct Trie* getNewTrieNode()
{
	struct Trie* node = (struct Trie*)malloc(sizeof(struct Trie));
	node->isEndOfWord = 0;

	for (int i = 0; i < CHAR_SIZE; i++)
		node->Children[i] = NULL;

	return node;
}

void print()
{
    printf("\n");
}

// Function to insert a string into a Trie
void insert(struct Trie *head, char* str)
{
	// start from root node
	struct Trie* curr = head;
	while (*str)
	{
		if (*str == 10)     //In case there is a new line character
		{
			break;
		}

		int index;
		if (*str == SPACE)
			index = 26;
		else
			index = *str - 'a';

		// create a new node if path doesn't exist
		if (curr->Children[index] == NULL)
			curr->Children[index] = getNewTrieNode();

		// go to next node

		curr = curr->Children[index];

		// move to next character
		str++;
	}

	// mark current node as leaf
	curr->isEndOfWord = 1;
}

/* Function to search a string in Trie. It returns 1 if
the string is found in the Trie, else it returns 0 */

int search(struct Trie* head, char* str)
{
	// returns 0 if Trie is empty
	if (head == NULL)
		return 0;

	struct Trie* curr = head;
	while (*str)
	{
		// go to next node
		curr = curr->Children[*str - 'a'];

		// if end of path is reached
		if (curr == NULL)
			return 0;

		// move to next character
		str++;
	}

	/* if current node is a leaf and we have reached the
	   end of the string, return 1 */

	return curr->isEndOfWord;
}

// returns 1 if given node has any children
int haveChildren(struct Trie* curr)
{
	for (int i = 0; i < CHAR_SIZE; i++)
		if (curr->Children[i])
			return 1;	// child found

	return 0;
}


// Recursive function to print auto-suggestions for given node.

void recSuggest(struct Trie* root, string pre)
{
    // found a string in Trie with the given prefix
    if (root->isEndOfWord)
    {
        cout << "\t" << pre;
        print();
    }

    // All children struct node pointers are NULL
    if (!haveChildren(root))
        return;

    for (int i = 0; i < CHAR_SIZE; i++)
    {
        if (root->Children[i])
        {
            // append current character to prefix string

			if (i == 26)      //if the character is a space
				pre.push_back(SPACE);
			else
            	pre.push_back(97 + i);

            recSuggest(root->Children[i], pre);
            // remove last character
            pre.pop_back();
        }
    }
}

// print suggestions for given prefix based on words present in the Trie
int autoSuggest(Trie* root, const string prefix)
{
    struct Trie* node = root;


    /*Check if prefix is present and find the
    the node with last character
    of given string. */
    
	int level,index;
    int n = prefix.length();
    for (level = 0; level < n; level++)
    {

		if (prefix[level] == SPACE)
			index = 26;
		else
			index = CHAR_TO_INDEX(prefix[level]);

        // no string in the Trie has this prefix
        if (!node->Children[index])
            return 0;

        node = node->Children[index];
    }

    // If prefix is present as a word.
    bool isWord = (node->isEndOfWord == true);

    // If it is the last node, i.e it has no children 

    bool isLast = !haveChildren(node);

    // If prefix contains the full word and no further children are found 

    if (isWord && isLast)
    {
        cout << "\t" << prefix << endl;
        return -1;
    }

    // If there are are nodes below last matching character
    if (!isLast)
    {
        string pref = prefix;
        recSuggest(node, pref);
        return 1;
    }
	return 1;
}

// Trie Implementation in C - Insertion and Searching
int main()
{
	struct Trie* dictRoot = getNewTrieNode();
	struct Trie* albumRoot = getNewTrieNode();
	struct Trie* songRoot = getNewTrieNode();
	struct Trie* artistRoot = getNewTrieNode();

	int ch;

	do
	{
		printf("1. Dictionary operations\n");
		printf("2. Music Finder\n");
		printf("3. Exit\n");
		printf("\tEnter your choice: ");
		scanf("%d",&ch);
		print();

		switch(ch)
		{
			case 1:
			{

				//Inputting words into the trie from the file "Dictionary.txt"
				FILE *fp1;

				fp1 = fopen("Dictionary.txt","r");

				if (!fp1)
				{
					printf("Error: Could not open file");
					exit(0);
				}
				char str[50];

				while(fgets(str,50,fp1) != NULL)
				{
					insert(dictRoot,str);
				}

				printf("Successfully populated the trie with the words in Dictionary.txt\n\n");

				char ch2;

				do
				{
					printf("\na. Input words into a trie\n");
                    printf("b. Check whether a word is present in the trie\n");
					printf("c. Search for words by showing autocomplete suggestions\n");
					printf("x. Exit\n\n");
					printf("\tEnter your choice: ");

					scanf("%c",&ch2);
					ch2 = getchar();

					printf("\n\n");

					switch(ch2)
					{
						case 'a':
						{

							char word[50];
							printf("Enter the word or phrase to be inserted into the trie: ");
							scanf("%s",word);

							insert(dictRoot,word);

							printf("%s has been successfully inserted\n",word);

							break;
						}

                        case 'b':
                        {
                            char word_check[50];
                            printf("Enter the word to be searched for: ");
                            scanf("%s",word_check);

                            int check = search(dictRoot,word_check);
                            if (check == 1)
                                printf("%s is present in the trie\n",word_check);
                            else
                                printf("%s is not present in the trie\n",word_check);

                            break;
                        }

						case 'c':
						{

							printf("Enter search keyword: ");
							char pre[50];
							scanf("%s",pre);
							printf("Search results:\n\n");

							int comp;
							comp = autoSuggest(dictRoot, pre);
							break;
						}

						case 'x':
							break;

						default:
							printf("Invalid choice\n");
							
					}
				} while (ch2 != 'x');



				break;
			}


			case 2:
			{

				char str[50];

				//Processing the music album names

				FILE *fp2;
				fp2 = fopen("albums.txt","r");

				if (!fp2)
				{
					printf("Error: Could not open file");
					exit(0);
				}

				while(fgets(str,50,fp2) != NULL)
				{
					insert(albumRoot,str);
				}

				//Processing songs

				FILE *fp3;
				fp3 = fopen("songs.txt","r");

				if (!fp3)
				{
					printf("Error: Could not open file");
					exit(0);
				}

				while(fgets(str,50,fp3) != NULL)
				{

					insert(songRoot,str);

				}

				//Processing music artist names

				FILE *fp4;
				fp4 = fopen("artists.txt","r");


				if (!fp4)
				{
					printf("Error: Could not open file");
					exit(0);
				}

				while(fgets(str,50,fp4) != NULL)
				{
					insert(artistRoot,str);
				}


				char pre[50];

				printf("Enter the search keyword for an album, song or artist: ");
				scanf("%s",pre);
				print();

				int comp;

				printf("===================\n");
				printf("\tALBUMS:\n");
				comp = autoSuggest(albumRoot, pre);

				printf("\n\tSONGS:\n");
				comp = autoSuggest(songRoot, pre);

				printf("\n\tARTISTS:\n");
				comp = autoSuggest(artistRoot, pre);
				printf("===================\n");

				if (comp == -1)
					cout << "No other strings found with this prefix\n";

				else if (comp == 0)
					cout << "No string found with this prefix\n";

				break;
			}

			case 3:
			{
				print();
				exit(0);
			}

			default:
				printf("Invalid choice");
		}
	} while (ch != 3) ;

	
    print();
	return 0;
}
