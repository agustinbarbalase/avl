#include "avl.h"
#include <string.h>
#include <malloc.h>

typedef struct node node_t;

struct node {
  const char *key;
  void *data;
  size_t balance;
  node_t *left;
  node_t *right;
};

typedef struct avl {
  node_t *root;
  size_t size;
  avl_compare_t cmp;
  avl_destroy_t destroy;
} avl_t;

void avl_init(avl_compare_t cmp, avl_destroy_t destroy) {
  avl_t *avl = malloc(sizeof(avl_t*));
  if (!avl) {
    return NULL;
  }

  avl->size = 0;
  avl->root = NULL;
  avl->cmp = cmp;
  avl->destroy = destroy;

  return avl;
}

size_t avl_size(avl_t *avl) {
  return avl->size;
}

void _avl_destroy(node_t* current, avl_destroy_t destroy) {
  if(!current) return;

  _avl_destroy(current->left, destroy);
  _avl_destroy(current->right, destroy);
  
  free(current->key);
  if(destroy) { 
    destroy(current->data); 
  }
  free(current);
}

void avl_destroy(avl_t *avl) {
  _avl_destroy(avl->root, avl->destroy);
  free(avl);
}
