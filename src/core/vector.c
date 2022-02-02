#include "core/vector.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INIT_SIZE 10

Vector *vector_create(size_t element_size)
{
    Vector *vector = malloc(sizeof(Vector));

    vector->element_size = element_size;
    vector->reserved_size = INIT_SIZE * element_size;
    vector->buffer_size = 0;
    vector->num_elements = 0;
    vector->buffer = malloc(vector->reserved_size);

    return vector;
}

void vector_destroy(Vector *vector)
{
    free(vector->buffer);
    free(vector);
}

void vector_push(Vector *vector, const void *element)
{
    if (vector->buffer_size >= vector->reserved_size)
    {
        vector->reserved_size = vector->buffer_size * 2;

        void *m = realloc(vector->buffer, vector->reserved_size);
        vector->buffer = m;
    }

    memcpy((char *)vector->buffer + vector->buffer_size, element, vector->element_size);
    vector->num_elements++;
    vector->buffer_size += vector->element_size;
}

void vector_erase(Vector *vector, int index)
{
    if (index != vector->num_elements - 1)
        memcpy((char *)vector->buffer + index * vector->element_size, (char *)vector->buffer + vector->buffer_size - vector->element_size, vector->element_size);

    vector->num_elements--;
    vector->buffer_size -= vector->element_size;
}

void vector_shrink(Vector *vector)
{
    vector->buffer = realloc(vector->buffer, vector->buffer_size);
    vector->reserved_size = vector->buffer_size;
}

int vector_empty(const Vector *vector)
{
    return (vector->num_elements == 0);
}