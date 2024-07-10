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

node_t* get_node(node_t* current, node_t** parent, const char *key, avl_compare_t cmp) {
  if(current && cmp(current->key, key) == 0) return current;

  *parent = current;
  
  if(current->left && cmp(current->key, key) > 0) {
    return get_node(current->left, key, parent, cmp);
  } else if(current->right) {
    return get_node(current->right, key, parent, cmp);
  }

  return NULL;
}

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

bool avl_insert(avl_t *avl, const char *key, void *data) {
  node_t* parent = NULL;
  node_t* node = get_node(avl->root, &parent, key, avl->cmp);
  
  if(!node) {
    node = new_node(key, data);
    if(!node) return false;
    
    if(!parent) {
      avl->root = node;
    }

    if(parent && avl->cmp(parent->key, key) > 0) {
      parent->left = node;
    } else if(parent) {
      parent->right = node;
    }

    avl->size++;
  } else {
    if(avl->destroy) {
      avl->destroy(node->data);
    }
    node->data = data;
  }
  
  return true;
}

void *avl_get(avl_t *avl, const char *key) {
  if(avl->size == 0) return NULL;
  node_t* parent = NULL;
  node_t* node = get_node(avl->root, &parent, key, avl->cmp);
  return node ? node->data : NULL;
}

bool avl_has_key(avl_t *avl, const char *key) {
  if(avl->size == 0) return false;
  node_t* parent = NULL;
  node_t* node = get_node(avl->root, &parent, key, avl->cmp);
  return node != NULL;
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
