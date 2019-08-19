#include<bits/stdc++.h>
using namespace std;

enum COLOR {red, black};

typedef struct node{
    int data, height, color, level;
    struct node *left, *right, *parent;
}nod;

node *root_bst = NULL, *root_rbt = NULL, *root_avl = NULL;
int path_arr[1000];

node* insert_bst(node *root, int data){
    if(!root){
        root = new node();
        root ->data = data;
        root -> left = root ->right = NULL;
        root -> height = 0;
    }
    else{
        if(root->data > data)
            root->left = insert_bst(root->left, data);
        else if(root -> data < data)
            root -> right = insert_bst(root->right, data);
    }
    root -> height = max(root->left->height, root -> right->height)+1;
    return root;
}

node* rotate_right_single(node *root);
node* rotate_left_single(node* root);
node* rotate_right_double(node* root);
node* rotate_left_double(node *root);

node* insert_avl(node *root, int data){
    if(!root){
        root = new node();
        root -> data = data;
        root -> height = 0;
        root -> left = root -> right = NULL;
    }
    else if(data < root -> data){
        root -> left = insert_avl(root -> left, data);
        if(root -> left -> height - root -> right -> height == 2){
            if(data < root -> left -> data)
                root = rotate_right_single(root);
            else root = rotate_right_double(root);
        }
    }
    else if(data > root->data){
        root -> right = insert_avl(root->right, data);
        if(root -> right->height - root->left->height == 2){
            if(data > root -> right->data)
                root = rotate_left_single(root);
            else root = rotate_left_double(root);
        }
    }
    root -> height = max(root->right->height, root->left->height) + 1;
    return root;
}

void rotate_left(node *&node);
void rotate_right(node *&node);

void fixrbt(node *&root, node *&node){
    struct node *parent = NULL, *grndprt = NULL, *uncle = NULL;
    while((node != root) && (node->parent->color == red) && (node->color != black)){
        parent = node -> parent;
        grndprt = node -> parent -> parent;

        if(parent == grndprt -> left){
            uncle = grndprt->right;
            if((uncle) && (uncle -> color == red)){
                parent -> color = uncle -> color = black;
                grndprt -> color = red;
                node = grndprt;
            }
            else{
                if(node == parent->right){
                    rotate_left(parent);
                    node = parent;
                    parent = node -> parent;
                }
                rotate_right(grndprt);
                swap(parent -> color, grndprt -> color);
                node = parent;
            }
        }
        else{
            uncle = grndprt->left;
            if(uncle && (uncle->color == red)){
                grndprt->color = red;
                parent -> color = uncle -> color = black;
                node = grndprt;
            }
            else{
                if(node == parent->left){
                    rotate_right(parent);
                    node = parent;
                    parent = node -> parent;
                }
                rotate_left(grndprt);
                swap(parent->color, grndprt->color);
                node = parent;
            }
        }
    }
    root_rbt -> color = black;
}

node* insert_rbt(node* root, int data){
    struct node *node = new struct node();
    node -> color = red; node -> parent = node -> left = node -> right = 0; node -> data = data; node->height = 0;
    if(!root)
        return node;
    if(node -> data < root -> data){
        root -> left = insert_rbt(root->left, node -> data);
        root -> left -> parent = root;
    }
    else if(node->data > root->data){
        root->right = insert_rbt(root->right, node->data);
        root -> right-> parent = root;
    }
    fixrbt(root, node);
    node->height = max(node->left->height, node->right->height)+1;
    return root;
}

void transverse_node(node *root, int flag){
    if(!root)
        return;
    transverse_node(root -> left, flag);
    if(flag == 1)
        root_avl = insert_avl(root_avl, root ->data);
    if(flag == 2)
        root_rbt = insert_rbt(root_rbt, root -> data);
    if(flag == 3)
        cout<<root->data<<" ";
    transverse_node(root->right, flag);   
}

void print_path(node* root, node* leaf){
    cout<<root->data;
    struct node* temp = root;
    while(root != leaf){
        if(root -> data > leaf -> data){
            root = root -> left;
            cout<<"->"<<root->data;
        }
        else if(root -> data < leaf -> data){
            root = root->right;
            cout<<"->"<<root-> data;
        }
    }
    cout<<"\n";
    root = temp;
    if(root -> data > leaf -> data){
        root = root -> left;
        print_path(root, leaf);
    }
    else if(root -> data < leaf -> data){
        root = root -> right;
        print_path(root, leaf);
    }
}

void find_leaf(node *root, int flag){
    if(!(root -> left) && !(root->right))
        if(flag == 1)
            print_path(root_bst, root);
        else if(flag == 2)
            print_path(root_avl, root);
        else if(flag == 3)
            print_path(root_rbt, root);
    if(root -> left)
        find_leaf(root->left, flag);
    else if(root -> right)
        find_leaf(root -> right, flag);
}

void find_level(node* node_parent, node *node){
    if(node == root_bst || node == root_avl || node == root_rbt)
        node->level = 0;
    else
        node -> level = node_parent->level +1;
    if(node->left)
        find_level(node, node->left);
    if(node->right)
        find_level(node, node->right);
}

void print_bst_avl(node *root, int flag){
        cout<<string(4 * root->level,' ');
        if(flag == 1)
            cout<<root->data<<"["<<root -> height<<"]"<<"\n";
        if(flag == 2)
            cout<<root->data<<"["<<(root->left->height - root->right->height)<<"]\n";
        if(flag == 3)
            cout<<root->data<<"["<<root->height<<"]"<<"["<<root-> color<<"]\n";
        if(root -> left)
            print_bst_avl(root -> left, flag);
        else if(!root->left && root->right)
            cout<<"\n";
        else if(root -> right)
            print_bst_avl(root -> right, flag);
        else if((root->left)&&!(root->right))
            cout<<"\n";
}

int main(){
    int n, i, flag, x, opt;
    
    cout<<"Enter number of terms\n";
    cin >> n;
    for(i = 0; i < n; i++){
        cin >> x;
        insert_bst(root_bst, x);
    }
    cout<<"1. To insert a node in the node and in the red-black tree\n"
    <<"2.  To create AVL tree from the inorder traversal of the node\n"
    <<"3.  To print the inorder traversal of the node/AVL/red-black tree\n"
    <<"4. To display all the paths in the node/AVL tree/red-black tree \n"
    <<"5.  To print the node/AVL tree/red-black Tree in the terminal using level-wise indentation\n"
    <<"6. Exit\n"
    <<"Enter option:\n";
    cin >> opt;
    if(opt == 1){
        cout<<"Enter node value\n";
        cin >> x;
        insert_bst(root_bst, x);
        insert_rbt(root_rbt, x);
    }
    else if(opt == 2){
        flag = 1;
        root_avl = NULL;
        transverse_node(root_bst, flag);
    }
    else if(opt == 3){
        transverse_node(root_bst, 3);
    }
    else if(opt == 4){
        find_leaf(root_bst, 1);
        find_leaf(root_avl, 2);
        find_leaf(root_rbt, 3);
    }
    else if(opt == 5){
        find_level(root_bst, NULL);
        find_level(root_avl, NULL);
        find_level(root_rbt, NULL);
        print_bst_avl(root_bst, 1);
        print_bst_avl(root_avl, 2);
        print_bst_avl(root_rbt, 3);
    }
}

node* rotate_right_single(node *root){
    node *left = root -> left;
    root -> left = left -> right;
    left -> right = root;
    root -> height = max(root -> right -> height, root -> left -> height) +1;
    left -> height = max(left -> right -> height, left -> left -> height) + 1;
    return left;
}

node* rotate_left_single(node* root){
    node* right = root -> right;
    root -> right = right -> left;
    right -> left = root;
    root -> height = max(root -> right -> height, root -> left -> height) +1;
    right -> height = max(right -> left -> height, right -> right -> height)+1;
    return right;
}

node *rotate_right_double(node *root){
    root -> left = rotate_left_single(root->left);
    return rotate_right_single(root);
}

node *rotate_left_double(node* root){
    root -> right = rotate_right_single(root -> right);
    return rotate_left_single(root);
}

void rotate_left(node *&node){
    struct node *node_right = node->right;
    node -> right = node_right -> left;
    if(node_right -> left)
        node_right -> left -> parent = node;
    if(!node -> parent)
        root_rbt = node_right;
    else if(node == node -> parent -> left)
        node -> parent -> left = node_right;
    else
        node->parent->right = node_right;
    node_right -> left = node;
    node_right -> parent = node -> parent;
    node -> parent = node_right;
}

void rotate_right(node *&node){
    struct node *node_left = node->left;
    node -> left = node_left -> right;
    if(node_left -> right)
        node_left->right->parent = node;
    if(!(node->parent))
        root_rbt = node_left;
    else if(node == node->parent->left)
        node -> parent ->left = node_left;
    else
        node -> parent->right = node_left;
    node_left -> right = node;
    node_left -> parent = node->parent;
    node -> parent = node_left;
}