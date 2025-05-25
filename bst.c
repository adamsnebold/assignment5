/*
 * This file contains an implementation of the basic BST functions prototyped
 * in bst.h. At the bottom of this file is where you will implement your
 * functions for this assignment. Make sure to add your name and
 * @oregonstate.edu email address below:
 *
 * Name:
 * Email:
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bst.h"
#include "stack.h"

/*
 * This structure represents a single node in a BST.
 */
struct bst_node {
  int val;
  struct bst_node* left;
  struct bst_node* right;
};

/*
 * This structure represents an entire BST. We only need a
 * reference to the root node of the tree.
 */
struct bst {
  struct bst_node* root;
};

struct bst* bst_create() {
  struct bst* bst = malloc(sizeof(struct bst));
  assert(bst);
  bst->root = NULL;
  return bst;
}

void bst_free(struct bst* bst) {
  assert(bst);
  while (!bst_isempty(bst)) {
    bst_remove(bst->root->val, bst);
  }
  free(bst);
}

int bst_isempty(struct bst* bst) {
  assert(bst);
  return bst->root == NULL;
}

struct bst_node* _bst_node_create(int val) {
  struct bst_node* n = malloc(sizeof(struct bst_node));
  assert(n);
  n->val = val;
  n->left = n->right = NULL;
  return n;
}

struct bst_node* _bst_subtree_insert(int val, struct bst_node* n) {
  if (n == NULL) {
    return _bst_node_create(val);
  } else if (val < n->val) {
    n->left = _bst_subtree_insert(val, n->left);
  } else {
    n->right = _bst_subtree_insert(val, n->right);
  }
  return n;
}

void bst_insert(int val, struct bst* bst) {
  assert(bst);
  bst->root = _bst_subtree_insert(val, bst->root);
}

int _bst_subtree_min_val(struct bst_node* n) {
  while (n->left != NULL) {
    n = n->left;
  }
  return n->val;
}

struct bst_node* _bst_subtree_remove(int val, struct bst_node* n) {
  if (n == NULL) {
    return NULL;
  } else if (val < n->val) {
    n->left = _bst_subtree_remove(val, n->left);
    return n;
  } else if (val > n->val) {
    n->right = _bst_subtree_remove(val, n->right);
    return n;
  } else {
    if (n->left != NULL && n->right != NULL) {
      n->val = _bst_subtree_min_val(n->right);
      n->right = _bst_subtree_remove(n->val, n->right);
      return n;
    } else if (n->left != NULL) {
      struct bst_node* left_child = n->left;
      free(n);
      return left_child;
    } else if (n->right != NULL) {
      struct bst_node* right_child = n->right;
      free(n);
      return right_child;
    } else {
      free(n);
      return NULL;
    }
  }
}

void bst_remove(int val, struct bst* bst) {
  assert(bst);
  bst->root = _bst_subtree_remove(val, bst->root);
}

int bst_contains(int val, struct bst* bst) {
  assert(bst);
  struct bst_node* cur = bst->root;
  while (cur != NULL) {
    if (val == cur->val) {
      return 1;
    } else if (val < cur->val) {
      cur = cur->left;
    } else {
      cur = cur->right;
    }
  }
  return 0;
}

/*****************************************************************************
 *
 * Below are the functions and structures implemented for this assignment.
 *
 *****************************************************************************/

struct bst_iterator {
  struct stack* s;
};

int _bst_subtree_size(struct bst_node* n) {
  if (n == NULL) return 0;
  return 1 + _bst_subtree_size(n->left) + _bst_subtree_size(n->right);
}

int bst_size(struct bst* bst) {
  assert(bst);
  return _bst_subtree_size(bst->root);
}

int _bst_subtree_height(struct bst_node* n) {
  if (n == NULL) return -1;
  int left_height = _bst_subtree_height(n->left);
  int right_height = _bst_subtree_height(n->right);
  return 1 + (left_height > right_height ? left_height : right_height);
}

int bst_height(struct bst* bst) {
  assert(bst);
  return _bst_subtree_height(bst->root);
}

int _bst_subtree_path_sum(int sum, struct bst_node* n) {
  if (n == NULL) return 0;

  // Subtract current node value from sum
  sum -= n->val;

  // If it's a leaf node, check if sum == 0
  if (n->left == NULL && n->right == NULL) {
    return (sum == 0);
  }

  // Otherwise, continue searching in left and right subtrees
  return _bst_subtree_path_sum(sum, n->left) || _bst_subtree_path_sum(sum, n->right);
}

int bst_path_sum(int sum, struct bst* bst) {
  assert(bst);
  return _bst_subtree_path_sum(sum, bst->root);
}

struct bst_iterator* bst_iterator_create(struct bst* bst) {
  assert(bst);
  struct bst_iterator* iter = malloc(sizeof(struct bst_iterator));
  assert(iter);
  iter->s = stack_create();
  struct bst_node* cur = bst->root;
  while (cur) {
    stack_push(iter->s, cur);
    cur = cur->left;
  }
  return iter;
}

void bst_iterator_free(struct bst_iterator* iter) {
  assert(iter);
  stack_free(iter->s);
  free(iter);
}

int bst_iterator_has_next(struct bst_iterator* iter) {
  assert(iter);
  return !stack_isempty(iter->s);
}

int bst_iterator_next(struct bst_iterator* iter) {
  assert(iter);
  struct bst_node* n = stack_pop(iter->s);
  int val = n->val;
  struct bst_node* cur = n->right;
  while (cur) {
    stack_push(iter->s, cur);
    cur = cur->left;
  }
  return val;
}
