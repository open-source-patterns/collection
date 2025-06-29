/**
 * @file array.h
 * @brief Concrete implementation of the IArray interface using a singly linked list.
 *
 * Defines the Array struct that implements the IArray interface,
 * along with allocation, initialization, creation, and free functions.
 *
 * This implementation provides thread-safe list operations.
 */
#pragma once

#include "iarray.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new IArray instance (heap-allocated).
 * @return Initialized IArray pointer, or NULL on failure.
 */
struct IArray *collection_array_new();

/**
 * @brief Frees a heap-allocated IArray.
 * @param array Pointer to the IArray pointer to free (sets *array to NULL).
 */
void collection_array_free(struct IArray **array);

#ifdef __cplusplus
}
#endif
