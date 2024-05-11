#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap* arbol = (TreeMap *)malloc(sizeof(TreeMap));
    arbol->root = NULL;
    arbol->current = NULL;
    arbol->lower_than = lower_than;
    return arbol;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode* newNode = createTreeNode(key, value);
    newNode->pair->key = key;
    newNode->pair->value = value;
    newNode->parent = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    
    if(tree->root == NULL)
    {
        tree->root = newNode;
        return;
    }

    TreeNode* parent = NULL;
    TreeNode* aux = tree->root;

    while(aux != NULL)
    {
        parent = aux;
        if(is_equal(tree, key, aux->pair->key))
        {
            aux->pair->value = value;
            free(newNode);
            return;
        }
        else if(tree->lower_than(key, aux->pair->key))
        {
            aux = aux->left;
        }
        else
        {
            aux = aux->right;
        }
    }

    newNode->parent = parent;

    if(tree->lower_than(key, parent->pair->key))
    {
        parent->left = newNode;
    }
    else
    {
        parent->right = newNode;
    }
    tree->current = newNode;
}

TreeNode * minimum(TreeNode * x){
    if(x == NULL) return NULL;

    while(x->left != NULL)
    {
        x = x->left;        
    }
    return x;
}
 
void removeNode(TreeMap * tree, TreeNode* node) {
    if(tree == NULL || node == NULL) return;

    TreeNode* parent = node->parent;

    if(node->left == NULL && node->right == NULL)
    {
        if(parent == NULL)
        {
            tree->root = NULL;
            return;
        }
        if(node == parent->left)
        {
            parent->left = NULL;
        }
        else
        {
            parent->right = NULL;
        }
        free(node);
    }
    else if(node->left == NULL || node->right == NULL)
    {
        TreeNode* child = (node->left != NULL) ? node->left : node->right;

        if(parent == NULL)
        {
            tree->root = child;
        }
        else if(node == parent->left)
        {
            parent->left = child;
        }
        else
        {
            parent->right = child;
        }
        child->parent = parent;
        free(node);
    }
    else
    {
        TreeNode* min = minimum(node->right);
        node->pair->key = min->pair->key;
        node->pair->value = min->pair->value;
        removeNode(tree, min);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* aux = tree->root;

    while(aux != NULL)
    {
        if(is_equal(tree, key, aux->pair->key))
        {
            tree->current = aux;
            return aux->pair;
        }
        else if(tree->lower_than(key, aux->pair->key))
        {
            aux = aux->left;
        }
        else
        {
            aux = aux->right;
        }
    }
    return NULL;
}

Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode* aux = tree->root;
    TreeNode* ub_node = NULL;

    while(aux != NULL)
    {
        if(tree->lower_than(key, aux->pair->key))
        {
            ub_node = aux;
            aux = aux->left;
        }
        else if(tree->lower_than(aux->pair->key, key))
        {
            aux = aux->right;
        }
        else
        {
            return aux->pair;
        }
    }    
    return ub_node != NULL ? ub_node->pair : NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if(tree->root == NULL) return NULL;
    if(tree->root->left == NULL) return tree->root->pair;

    TreeNode* min = minimum(tree->root->left);
    
    return min->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode* parent = tree->current->parent;

    if(tree->current == tree->root && tree->root->right == NULL) return NULL;
    
    if(tree->current->right == NULL)
    {
        while(parent != NULL)
            {
                if(tree->lower_than(tree->current->pair->key, parent->pair->key))
                {
                    tree->current = parent;
                    return parent->pair;
                }
                else
                {
                    parent = parent->parent;
                }
            }
    }
    else
    { 
        TreeNode* min = minimum(tree->current->right);
        tree->current = min;
        return min->pair;
    }
    return NULL;
}
