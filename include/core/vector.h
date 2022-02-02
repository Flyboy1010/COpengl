#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stddef.h>

// return the element i of the vector v of the type T

#define vector_element(T, v, i) ((T*)v->buffer)[i]

typedef struct Vector
{
    void *buffer;
    size_t element_size;
    size_t buffer_size;
    size_t reserved_size;
    int num_elements;
} Vector;

Vector *vector_create(size_t element_size);
void vector_destroy(Vector *vector);

void vector_push(Vector *vector, const void *element);
void vector_erase(Vector *vector, int index);
void vector_shrink(Vector *vector);
int vector_empty(const Vector *vector);

#endif // _VECTOR_H_