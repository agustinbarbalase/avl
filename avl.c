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
