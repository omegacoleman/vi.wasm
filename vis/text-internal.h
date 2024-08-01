#ifndef TEXT_INTERNAL
#define TEXT_INTERNAL

#include <stdbool.h>
#include <stddef.h>
#include "text.h"

/* Block holding the file content,
 * heap allocated to store the modifications.
 */
typedef struct {
	size_t size;               /* maximal capacity */
	size_t len;                /* current used length / insertion position */
	char *data;                /* actual data */
} Block;

Block *block_alloc(size_t size);
Block *block_read(size_t size, int fd);
Block *block_load(const char *filename, enum TextLoadMethod method, struct stat *info);
void block_free(Block*);
bool block_capacity(Block*, size_t len);
const char *block_append(Block*, const char *data, size_t len);
bool block_insert(Block*, size_t pos, const char *data, size_t len);
bool block_delete(Block*, size_t pos, size_t len);

void text_saved(Text*, struct stat *meta);

#endif
