#include <iostream>

using namespace std;

struct Node
{
    int key;
    struct Node *left;
    struct Node *right;
    int height;
};

int height(struct Node* node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

int getBalance(struct Node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

//quay trai
struct Node* leftRotate(struct Node *x)
{
    struct Node *y = x->right;
    struct Node *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Return new root
    return y;
}

//quay phai
struct Node* rightRotate(struct Node *y)
{
    struct Node *x = y->left;
    struct Node *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    //  Update heights
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Return new root
    return x;
}

struct Node* insert(struct Node* root, int key)
{
    /* 1. Perform the normal BST insertion */
    if (root == NULL)
        return(new Node{key, NULL, NULL, 1});

    if (key < root->key)
        root->left  = insert(root->left, key);
    else if (key > root->key)
        root->right = insert(root->right, key);
    else // Equal keys are not allowed in BST
        return root;

    /* 2. Update height of this ancestor node */
    root->height = 1 + max(height(root->left),
                           height(root->right));

    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = getBalance(root);

    // If this node becomes unbalanced, then
    // there are 4 cases

    // Left Left Case
    if (balance > 1 && key < root->left->key)
        return rightRotate(root);

    // Right Right Case
    if (balance < -1 && key > root->right->key)
        return leftRotate(root);

    // Left Right Case
    if (balance > 1 && key > root->left->key)
    {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Left Case
    if (balance < -1 && key < root->right->key)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    /* return the (unchanged) node pointer */
    return root;
}

struct Node* minValueNode(struct Node* node)
{
    struct Node* current = node;

    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;

    return current;
}

struct Node* deleteNode(struct Node* root, int key)
{
    // Step 1 - Perform standard BST delete
    if (root == NULL)
        return root;

    // If the key to be deleted is smaller than the
    // root's key, then it lies in left subtree
    if ( key < root->key )
        root->left = deleteNode(root->left, key);

    // If the key to be deleted is greater than the
    // root's key, then it lies in right subtree
    else if( key > root->key )
        root->right = deleteNode(root->right, key);

    // if key is same as root's key, then this is the node
    // to be deleted
    else
    {
        // node with only one child or no child
        if( (root->left == NULL) || (root->right == NULL) )
        {
            struct Node *temp = root->left ? root->left : root->right;

            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
            *root = *temp; // Copy the contents of the non-empty child
            delete(temp);
        }
        else
        {
            // node with two children: Get the inorder successor (smallest
            // in the right subtree)
            struct Node* temp = minValueNode(root->right);

            // Copy the inorder successor's data to this node
            root->key = temp->key;

            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->key);
        }
    }

    // If the tree had only one node then return
    if (root == NULL)
      return root;

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(height(root->left),
                           height(root->right));

    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to check whether
    //  this node became unbalanced)
    int balance = getBalance(root);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

bool in(int x, Node *S) 
{
	if (!S)
	return false;
	else	if (S->key == x)return true;
	else if (S->key> x) return in(x,S->left);
	else return in(x,S->right);
}

void print(Node *S) 
{
	if (S) 
	{
		print(S->left);
		cout <<S->key <<" ";
		print(S->right);
	}	
	cout << endl;
}

//Check a set of integers is subset a other set
bool subset(Node* S1, Node* S2)
{
	if (!S1) return true;
	else
	return subset(S1->left, S2) && subset(S1->right, S2) && in(S1->key, S2);
}

//intersection of two sets
Node* intersectionSet(Node* S1, Node* S2, Node* S3)
{
	if (S1!= NULL)
	{
		if (in(S1->key, S2))insert(S3, S1->key);
		S3 = intersectionSet(S1->left, S2, S3);
		S3 = intersectionSet(S1->right, S2, S3);
    }
	return S3;
}

//Append a set into another set
Node* append(Node* S1, Node* S2)
{
	if (S1)
	{
		S2 = insert(S2, S1->key);
		S2 = append(S1->left, S2);
		S2 = append(S1->right, S2);
	}
	return S2;
}

//Union of two sets
Node* unionSet(Node* S1, Node*S2)
{
	Node* S3 = NULL;
	S3 = append(S1, S3);
	S3 = append(S2, S3);
	return S3;
}

int main()
{
	Node *S1 = NULL, *S2 = NULL, *S3 = NULL;
	int i, n = 10;
	//Insert to S1
	for(i = 1; i <= n; i++)
	{
		S1 = insert(S1, i);
	}
	cout << "S1: " ; print(S1); cout << endl;
	//Insert to S2
	for(i = n/2; i <= 2*n; i++)
	{
		S2 = insert(S2, i);
	}
	cout << "S2: " ; print(S2); cout << endl;
	bool b = subset(S1, S2);
	if (b)	cout << "S1 is a subset of S2" << endl;
	else	cout << "S1 is not a subset of S2" << endl;
	S3 = intersectionSet(S1, S2, S3);
	cout << "S3: " ; print(S3); cout << endl;
}


