#include <stdbool.h>
#include <stdlib.h>

typedef struct avl avl_t;
typedef int (*avl_compare_t)(const char *, const char *);
typedef void (*avl_destroy_t)(void *);

void avl_init(avl_compare_t cmp, avl_destroy_t destroy);

bool avl_insert(avl_t *avl, const char *key, void *data);

void *avl_delete(avl_t *avl, const char *key);

void *avl_get(avl_t *avl, const char *key);

bool avl_has_key(avl_t *avl, const char *key);

size_t avl_size(avl_t *avl);

void avl_destroy(avl_t *avl);
