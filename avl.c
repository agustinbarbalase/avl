#define _POSIX_C_SOURCE 200809L 
#include "avl.h"
#include <string.h>
#include <malloc.h>

typedef struct node node_t;

struct node {
  char *key;
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
  if(!current) return NULL;

  if(cmp(current->key, key) == 0) return current;

  *parent = current;
  
  if(cmp(current->key, key) > 0) {
    return get_node(current->left, parent, key, cmp);
  } else if(cmp(current->key, key) < 0) {
    return get_node(current->right, parent, key, cmp);
  }

  return NULL;
}

node_t* new_node(const char* key, void* data) {
  node_t* node = malloc(sizeof(node_t));
  if(!node) return NULL;

  node->key = strdup(key);
  if(!node->key) {
    free(node);
    return NULL;
  }
  
  node->data = data;
  node->balance = 0;
  node->left = NULL;
  node->right = NULL;

  return node;
}

avl_t* avl_init(avl_compare_t cmp, avl_destroy_t destroy) {
  avl_t *avl = malloc(sizeof(avl_t));
  if (!avl) {
    return NULL;
  }

  avl->size = 0;
  avl->root = NULL;
  avl->cmp = cmp;
  avl->destroy = destroy;

  return avl;
}

bool _avl_insert(avl_t* avl, node_t* current, node_t* parent, const char* key, void* data) {
  if(!current) {
    node_t* node = new_node(key, data);
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

    return true;
  } 

  if(current && avl->cmp(current->key, key) == 0) {
    if(avl->destroy) {
      avl->destroy(current->data);
    }
    current->data = data;
    return true;
  }

  if(current && avl->cmp(current->key, key) > 0) {
    if(!_avl_insert(avl, current->left, current, key, data)) return false;
  } else if(current && avl->cmp(current->key, key) < 0) {
    if(!_avl_insert(avl, current->right, current, key, data)) return false;
  }
}

bool avl_insert(avl_t *avl, const char *key, void *data) {
  return _avl_insert(avl, avl->root, NULL, key, data);
}

node_t* get_replacement_node(node_t* current) {
  node_t* replacement_node = current->left;

  while(replacement_node->right) {
    replacement_node = replacement_node->right;
  }

  return replacement_node;
}

void *avl_delete(avl_t *avl, const char *key) {
  node_t* parent = NULL;
  node_t* node = get_node(avl->root, &parent, key, avl->cmp);
  if(!node) return;

  void* data = node->data;
  if(node->left && node->right) {
    node_t* replacement_node = get_replacement_node(node);
    char* replacement_key = strdup(replacement_node->key);
    void* replacement_data = avl_delete(avl, replacement_node->key);
    free(node->key);

    node->key = replacement_key;
    node->data = replacement_data;
  } else {
    node_t* new_child = node->left ? node->left : node->right;
    
    if(!parent) {
      avl->root = new_child;
    } else {
      if(parent->left && avl->cmp(parent->left->key, node->key) == 0) {
        parent->left = new_child;
      } else if(parent->right) {
        parent->right = new_child;
      }
    }
    
    free(node->key);
    free(node);
    avl->size--;
  }

  return data;
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
