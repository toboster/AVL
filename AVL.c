/**
 * CS-361
 * Author: Tony Nguyen
 * Partner: Tyler Fenske
 *
 * Given a text file will build AVL tree by performing
 * AVL inserts on every node.
 *
 * input txt file is expected to have a integer at the 
 * top of the file indicating the size of input file,
 * thereafter the data will follow where each unique
 * integer should be on its own line.
 *
 * The program allows user to insert, search, delete, or
 * display from the console. Specific instructions are given
 * when program is run.

 * Tyler :
 * Each of the main functions 
 * {building, inserting, searching, and deletion}
 * provide a runtime analysis to the console.
 *
 **/
#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

struct TreeNode 
{
  int data;
  int height;
  int balanceFactor;
  struct TreeNode* left;
  struct TreeNode* right;
};

//text file w/dataSet, read in from cmd line arg
char* dataSet;

//used to keep track of runtime
time_t start, end;

//Only turns on if command line was input incorrectly
short helpFlag = 0;

/**
 * Tyler
 * Reads in data set's file name from command line args.
 **/
void readCommandLine(int argc, char **argv)
{
  int c;

  while ( (c = getopt(argc, argv, "f:")) != -1)
  {
    switch(c)
    {
      case 'f':
        dataSet = optarg;
        break;
      case '?':
        helpFlag = 1;
        break;
      default:
        helpFlag = 1;
    }
  }
}

/* ************************************
 * Each parameter ’s type and name :
 int data
 * Function ’s return value .
 struct TreeNode*
 * Description of what the function
 * does .
 Allocates memory for a new node, assigns data to struct.
 ************************************ */
struct TreeNode* createNode(int data)
{
  struct TreeNode* node = malloc(sizeof(struct TreeNode));
  node->balanceFactor = 0; 
  node->height = 1;
  node->data = data;
  node->left = NULL;
  node->right = NULL;

  return node;
}

/**
 * Tyler
 * Searches to see if value is already in the bst using recursion.
 * Returns 1 if it is.
 * Else returns 0.
 **/
short searchAVL(struct TreeNode* root, int value)
{
  short result;

  if(root == NULL)
  {
    return 0;
  }
  if(value == root->data)
  {
    return 1;
  }
  else if(value < root->data)
  {
    result = searchAVL(root->left, value);
  }
  else if(value > root->data)
  {
    result = searchAVL(root->right, value);
  }

  return result;
}

/* ************************************
 * Each parameter ’s type and name :
 struct TreeNode* node
 * Function ’s return value .
 struct TreeNode*  
 * Description of what the function
 * does .
 Grabs left most node, used to help RemovedNode method.
 * Function ’s algorithm
 1) Keeps stepping through left nodes until NULL
 2) Reassigns left most node if relative right node exist
 3) Returns left most node.
 ************************************ */
struct TreeNode* grabLeftmost(struct TreeNode* node)
{
  struct TreeNode* curr = NULL;
  struct TreeNode* prev = NULL;
  /* there is no right node*/ 
  if(node == NULL) return node;
  curr = node; 
  /* look for left most node */ 
  while(curr->left)
  {
    prev = curr;
    curr = curr->left; 
  }
  /* only if there was a left node, assign in order succesor of left node */
  if(prev) 
  {
    prev->left = curr->right;
  }
  return curr;
}

int bestMax(int leftMax, int rightMax)
{
  return leftMax <= rightMax ? rightMax : leftMax; 
}

/* ************************************
 * Each parameter ’s type and name :
 struct TreeNode* root, int*height
 * Function ’s return value .
 void 
 * Description of what the function
 * does .
 recursively calls it itself by going through left and
 right nodes.
 * Function ’s algorithm
 1) localy will keep track of a left / right max.
 2) recursively step through left and right.
 3) dereference given height and changes value
 based on leftMax / rightMax + 1.
 ************************************ */
void maxDepth(struct TreeNode* root, int* height)
{
  int leftMax = 0;
  int rightMax = 0;
  if(root == NULL) return;
  // send pointers of local left and right maxes.
  maxDepth(root->left,&leftMax);
  maxDepth(root->right,&rightMax);
  // assigns best height.
  // result will be off by 1. 
  *height = bestMax(leftMax,rightMax) + 1;
}


/* ************************************
 * Each parameter ’s type and name :
 struct TreeNode* root
 * Function ’s return value .
 int
 * Description of what the function
 * does .
 Used to calculate the depth by using maxDepth function,
 will be off by 1.
 * Function ’s algorithm
 ************************************ */
int maxDepthHelper(struct TreeNode* root)
{
  int height = 0;
  maxDepth(root,&height);
  return height - 1;
}

/* ************************************
 * Each parameter ’s type and name :
 struct TreeNode* root
 * Function ’s return value .
 int
 * Description of what the function
 * does .
 Used to obtain the height based on the value
 within given struct.
 * Function ’s algorithm
 ************************************ */

int getheight(struct TreeNode* root)
{
  if(root == NULL)
  {
    return 0;
  }
  return root->height;
}

/* ************************************
 * Each parameter ’s type and name :
 struct TreeNode*root 
 * Function ’s return value .
 int
 * Description of what the function
 * does .
 Calculates the balance Factor by subtracting right depth
 by left depth.
 * Function ’s algorithm
 1) Calculates the balance Factor by subtracting right depth.
 ************************************ */
int getBalancFactor(struct TreeNode*root)
{
  if(root == NULL)
  {
    return 0;
  }
  return getheight(root->right) - getheight(root->left);
}

/* ************************************
 * Each parameter ’s type and name :
 struct TreeNode* root, 
 * Function ’s return value .
 struct TreeNode*
 * Description of what the function
 * does .
 performs a "left" rotation on given root.
 * Function ’s algorithm
 1) Remembers roots right node.
 2) reassign roots right node to root right lefts 
 node.
 3) assigns roots right node left to root.
 4) return new head node of root.
 ************************************ */
struct TreeNode* leftRotate(struct TreeNode*root)
{
  struct TreeNode* rootRight = root->right;
  // assign roots right child to rootRights left node.
  root->right = rootRight->left; 
  rootRight->left = root;
  // updates heights.
  root->height = bestMax(getheight(root->left),
      getheight(root->right)) + 1;
  rootRight->height = bestMax(getheight(rootRight->left),
      getheight(rootRight->right)) + 1;

  return rootRight;
}

/* ************************************
 * Each parameter ’s type and name :
 struct TreeNode* root, 
 * Function ’s return value .
 struct TreeNode*
 * Description of what the function
 * does .
 performs a "right" rotation on given root.
 * Function ’s algorithm
 1) Remembers roots left node.
 2) reassign roots left node to root lefts right
 node.
 3) assigns roots left node right to root.
 4) return new head node of root.
 ************************************ */
struct TreeNode* rightRotate(struct TreeNode*root)
{
  struct TreeNode* rootLeft = root->left;
  // assign roots left child to rootLefts right node.  
  root->left = rootLeft->right;
  rootLeft->right = root;
  // updates heights 
  root->height = bestMax(getheight(root->left),
      getheight(root->right)) + 1;
  rootLeft->height = bestMax(getheight(rootLeft->left),
      getheight(rootLeft->right)) + 1;

  return rootLeft;
} 

/* steps through nodes comparing desired data to
   current nodes data, if null, a spot is found where a new
   node can be created with desired data.*/
/* ************************************
 * Each parameter ’s type and name :
 struct TreeNode* root, int data
 * Function ’s return value .
 struct TreeNode*
 * Description of what the function
 * does .
 1) performs standard BST insertion.
 2) calculates the balance factors.
 3) performs rotations based on balance factor of local root.
 Inserts new data in binary search tree.
 * Function ’s algorithm
 1) Recursively steps through nodes comparing desired data to
 current nodes data 
 2) if null, a spot is found where a new
 node can be created with desired data.
 3) returning from function calls, calculates the balance factor
 for root, if it's 2 or -2 then balance current root, 2 is a 
 tree weighted on the right side. -2 is tree weighted on the left side.
 ************************************ */
struct TreeNode* insertAVL(struct TreeNode* root, int data)
{
  // standard BST insert.
  if(root == NULL)
  {
    return createNode(data);
  }
  else if(data < (root->data))
  {
    root->left = insertAVL(root->left, data);
  }
  else if(data > (root->data))
  {
    root->right = insertAVL(root->right, data);
  }

  // updates the height.
  root->height = bestMax(getheight(root->right), getheight(root->left)) + 1;
  // calculates the balance factor which is based on left/right of root.
  int new_balFactor = getBalancFactor(root);
  root->balanceFactor = new_balFactor;
  // we have a right case.
  if(new_balFactor > 1)
  {
    struct TreeNode* rootRight = root->right;
    // we have right-right case.
    if(data > root->right->data)
    {
      root = leftRotate(root);  
    }
    // we have right-left case.
    else if(data < root->right->data)
    {

      root->right = rightRotate(root->right);
      root = leftRotate(root);
    }
    else
    {
      printf("Error, unexpected balF right case :%d\n",rootRight->balanceFactor);
    }
  }
  // we have a left case.
  else if(new_balFactor < -1)
  {
    struct TreeNode* rootLeft = root->left;
    // we have left-right case.
    if(data > root->left->data)
    {
      root->left = leftRotate(root->left);  
      root = rightRotate(root);
    }  
    // we have left-left case.
    else if(data < root->left->data)
    {
      root = rightRotate(root);
    }
    else
    {
      printf("Error, unexpected balF left case :%d\n",rootLeft->balanceFactor);
    }
  }

  return root;
}

/* ************************************
 * Each parameter ’s type and name :
 struct TreeNode* node, int data, int* status
 * input and/or output ,
 status = 1 if node was found and vice versa
 * Function ’s return value .
 struct TreeNode*
 * Description of what the function
 * does .
 1) performs standard BST deletion.
 2) calculates balance factors of ancestor node.
 3) based on balance factors will perform rotations if needed to
 balance.
 Removes node with containing data, used within removeBST method.
 * Function ’s algorithm
 1) Traverses tree until data is found, if nothing
 is found does nothing.
 2) If found goes through different cases depending
 on nodes children. (see comments below).
 3) Calculates balance factor for current stack node, if 
 balance factor is -2 or 2 then balance current node.
 3) Returns head node. 
 ************************************ */
struct TreeNode* removeAVL(struct TreeNode* node, int data, int* status)
{
  // standard BST deletion.
  if(node == NULL) return node;
  else if(data < (node->data))
  {
    node->left = removeAVL(node->left, data, status);
  }
  else if(data > (node->data))
  {
    node->right = removeAVL(node->right, data, status);
  }
  else if(data == (node->data))
  {
    /* no children */
    if(node->right== NULL && node->left == NULL)
    {
      free(node);
      return NULL;
    }
    /* cases of 1 children and no children, left does not exist */
    if(node->left == NULL)
    {
      /* remembers node */
      struct TreeNode* temp = node;
      /* assigns node to inorder succesor */
      node = node->right;
      /* sucess deleting node */
      *status = 1;
      free(temp);
    }
    /* similar to above case but nodes right does not exist */
    else if(node->right == NULL)
    {
      struct TreeNode* temp = node;
      node = node->left;
      /* success deleting node */
      *status = 1;
      free(temp);
    }
    /* case of 2 children */
    else
    {
      /* pointers to keep track of relevant nodes */
      struct TreeNode* temp = node;
      struct TreeNode* right = node->right;
      struct TreeNode* left = node->left;
      /* node needs to become in order successor, left most node of current 
         right is needed and is assigned to node */
      node = grabLeftmost(right); 
      /* rebulding left node for new node from line above */ 
      node->left = left;
      /* if nodes in order succesor wasn't right, rebuilt right node */  
      if(node != right) 
      {
        node->right = right;
      } 
      /* sucess deleting node */
      *status = 1; 
      free(temp);
    }
  }

  // updates the height.
  node->height = bestMax(getheight(node->right), getheight(node->left)) + 1;
  // calculates the balance factor.
  int new_balFactor = getBalancFactor(node);
  node->balanceFactor = new_balFactor;

  if(node->data == 5)
  {
    printf("height 2 child : %d\n",node->height);
    printf("balFactor 2 child : %d\n",new_balFactor);
  }
  // we have a right case.
  if(new_balFactor > 1)
  {
    struct TreeNode* rootRight = node->right;
    // we have right-right case.
    if(data > node->right->data)
    {
      node = leftRotate(node);  
    }
    // we have right-left case.
    else if(data < node->right->data)
    {

      node->right = rightRotate(node->right);
      node = leftRotate(node);
    }
    else
    {
      printf("Error, unexpected balF right case :%d\n",rootRight->balanceFactor);
    }
  }
  // we have a left case.
  else if(new_balFactor < -1)
  {
    struct TreeNode* rootLeft = node->left;
    // we have left-right case.
    if(data > node->left->data)
    {
      node->left = leftRotate(node->left);  
      node = rightRotate(node);
    }  
    // we have left-left case.
    else if(data < node->left->data)
    {
      node = rightRotate(node);
    }
    else
    {
      printf("Error, unexpected balF left case :%d\n",rootLeft->balanceFactor);
    }
  }

  return node;
}

/* ************************************
 * Each parameter ’s type and name :
 struct TreeNode* node
 * Function ’s return value .
 void
 * Description of what the function
 * does .
 Given current node will recalculate the balance 
 factor by grabing maximum height of children.
 ************************************ */
void updateBalFactor(struct TreeNode* node)
{
  node->height = bestMax(getheight(node->right), getheight(node->left)) + 1;
  // calculates the balance factor.
  int new_balFactor = getBalancFactor(node);
  node->balanceFactor = new_balFactor;
}

/* ************************************
 * Each parameter ’s type and name :
 struct TreeNode* node
 * Function ’s return value .
 void
 * Description of what the function
 * does .
 Recursively goes through each node, and updates nodes
 balance factor.
 ************************************ */
void updateBalFactorUtil(struct TreeNode* node)
{
  if(node != NULL)
  {
    updateBalFactorUtil(node->left);
    updateBalFactor(node);
    updateBalFactorUtil(node->right);  
  }
}

/* ************************************
 * Each parameter ’s type and name :
 struct TreeNode* node
 * Function ’s return value .
 void
 * Description of what the function
 * does .
 Recursively goes through each node, and prints node
 In order traversal.
 ************************************ */
void printNode(struct TreeNode* node)
{
  if(node != NULL)
  {
    printNode(node->left);
    printf("%d ", node->data);
    printNode(node->right);
  }

}

/* ************************************
 * Each parameter ’s type and name :
 struct TreeNode* root
 * Function ’s return value .
 void
 * Description of what the function
 * does .
 Uses printNode method to print tree, then prints a new line.
 ************************************ */
void printTree(struct TreeNode* root)
{
  printNode(root);
  printf("\n");
}

/**
 * Tyler
 * Prints the bst to the console.
 * Node Value => The integer value stored at that node, also used to identify the node.
 * Height => The height of the current node in the bst.
 *
 * Example:
 *               6           height = 0
 *             5   7         height = 1
 *           3       9       height = 2
 *         2   4   8         height = 3
 **/
void displayUtil(struct TreeNode* root, int height)
{
  if(root == NULL)
  {
    return;
  }

  printf("\n%d\t%d\t%d\t\n", height, root->data, root->balanceFactor);

  height++;

  displayUtil(root->right, height);
  displayUtil(root->left, height);
}

/**
 * Written by Tyler.
 * Starting function for display that initializes the
 * height to 0, and prints the header of the display.
 **/
void displayChain(struct TreeNode* root)
{
  updateBalFactorUtil(root);
  printf("\nHeight\tNode \tNew Balance Factor\n\n");
  displayUtil(root, 0);
}

/* ************************************
 * Each parameter ’s type and name :
 struct TreeNode* root
 * Function ’s return value .
 void
 * Description of what the function
 * does .
 Recursively goes through each node and frees node. 
 ************************************ */
void freeTree(struct TreeNode* root)
{

  if (root == NULL) return;
  freeTree( root->left );
  freeTree( root->right );
  free( root );
  root = NULL;

}

/**
 * Tyler
 * If value user enters exists in tree, it is deleted.
 * Prints the runtime for deletion.
 **/
void userDelete(struct TreeNode** root)
{
  int de, result;
  //scanf detects newline, so need x var to trash the newline
  char keepGoing, x;
  double time;
  int status = 0;

  do
  {
    printf("\n\n\nWhat number would you like to delete out of the bst?\n");
    scanf("%d", &de);

    result = searchAVL(*root, de);

    if(result)
    {
      start = clock();
      *root = removeAVL(*root, de, &status);
      end = clock();
      printf("Value %d deleted from the tree.\n", de);
    }
    else
    {
      printf("Value not found, no entries deleted.\n");
    }

    time = (double)(end-start)/CLOCKS_PER_SEC;

    if(time == 0)
    {
      printf("\nRuntime for deletion: shorter than .000001 seconds.\n\n");
    }
    else
    {
      printf("\nRuntime for deletion in seconds: %f\n\n", time);
    }

    printf("Keep Deleting?\n"
        "y for yes\n");

    scanf("%c%c", &x, &keepGoing);

  }while(keepGoing == 'y');
}

/**
 * Tyler
 * If value user enters doesn't exist in the tree, it is inserted.
 * Prints the runtime for insertion.
 **/
void userInsert (struct TreeNode* root)
{
  int ins;
  char keepGoing, x;
  double time;

  do
  {
    printf("\n\n\nWhat number would you like to insert into the bst?\n");

    scanf("%d", &ins);

    start = clock();
    if(!(searchAVL(root, ins)))
    {
      insertAVL(root, ins);
      end = clock();
      printf("\nValue %d inserted into tree.\n", ins);
    }
    else
    {
      end = clock();
      printf("\nValue %d already exists in the tree.\n", ins);
    }

    time = (double)(end-start)/CLOCKS_PER_SEC;

    if(time == 0)
    {
      printf("\nRuntime for insert: shorter than .000001 seconds.\n\n");
    }
    else
    {
      printf("\nRuntime for insert in seconds: %f\n\n", time);
    }

    printf("Keep Inserting?\n"
        "y for yes\n");

    scanf("%c%c", &x, &keepGoing);

  }while(keepGoing == 'y');
} 

/**
 * Tyler
 * The user is notified whether or not the value the user entered exists in the tree.
 * Prints the runtime for searching.
 **/
void userSearch (struct TreeNode* root)
{
  int se, result;
  char keepGoing, x;
  double time;

  do
  {
    printf("\n\n\nWhat number would you like to search the bst for?\n");
    scanf("%d", &se);

    start = clock();
    result = searchAVL(root, se);
    end = clock();

    if(result)
    {
      printf("Search found value %d stored in the tree.\n", se);
    }
    else
    {
      printf("Search found no results.\n");
    }

    time = (double)(end-start)/CLOCKS_PER_SEC;

    if(time == 0)
    {
      printf("\nRuntime for search: shorter than .000001 seconds.\n\n");
    }
    else
    {
      printf("\nRuntime for search in seconds: %f\n\n", time);
    }

    printf("Keep Searching?\n"
        "y for yes\n");

    scanf("%c%c", &x, &keepGoing);

  }while(keepGoing == 'y');
}

/**
 * Tyler
 * Gives the user options to pick from to make changes to the bst.
 **/
void userControl(struct TreeNode** root)
{
  char userInput;

  while(userInput != 'q')
  {
    printf("\n\n\nWhat would you like to do now?\n\n\n"
        "Enter one of the following commands:\n\n"
        "Key Press\tAction\n\n"
        "s\t\tSearch Tree\n"
        "i\t\tInsert in Tree\n"
        "d\t\tDelete from Tree\n"
        "p\t\tPrint Tree Info\n"
        "q\t\tQuit Program\n\n");


    scanf("\n%c", &userInput);

    switch(userInput)
    {
      case 's':
        userSearch(*root);
        break;
      case 'i':
        userInsert(*root);
        break;
      case 'd':
        userDelete(root);
        break;
      case 'p':
        displayChain(*root);
        break;
      case 'q':
        break;
      default:
        printf("\n\n\nPlease choose an appropriate option {s, i, d, p, q}\n");
        break;

    }
  }
}

/**
 * Tyler
 * Reads in one line of dataset at a time, and inserts the
 * value read in from the file if it's not already in the tree.
 **/
struct TreeNode* readOneLine(char* singleLine, struct TreeNode* root)
{
  int val = atoi(singleLine);

  return insertAVL(root, val);
}


/**
 * Tyler
 * Main function to control building and manipulating the bst.
 **/
int main(int argc, char **argv)
{
  readCommandLine(argc, argv);

  if(helpFlag || argc == 1)
  {
    printf("Please run program in the following format:\n\n"
        "./bst -f DataSetn.txt\n\n");
    return 0;
  }

  double runtime;

  if(dataSet != NULL)
  {
    start = clock();

    FILE* fPointer = fopen(dataSet, "r");

    if(!fPointer)
    {
      printf("\nPlease rerun program with a valid file name.\n\n");
      return 0;
    }

    struct TreeNode* root = NULL;

    char singleLine[100];

    fgets(singleLine, 100, fPointer); 
    // first line of header is size of dataset which isn't useful
    // for the bst, so we trash it

    while (fgets(singleLine, 100, fPointer)) 
      // reads from fPointer till \n is found, 
      // stores everything in singleLine array
    {
      root = readOneLine(singleLine, root);
    }
    fclose(fPointer);

    end = clock();

    printf("\nBinary Search Tree built successfully!");
    runtime = (double)((end-start)/CLOCKS_PER_SEC);

    if(runtime == 0)
    {
      printf("\nRuntime to build tree: shorter than .000001 seconds.\n");
    }
    else
    {
      printf("\nRuntime to build tree in seconds: %f\n", runtime);
    }

    userControl(&root); 
    freeTree(root);
  }


}





