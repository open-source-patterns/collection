/**
 * @file dictionary.h
 * @brief Factory and destructor functions for IDictionary implementations.
 */

#pragma once

#include "idictionary.h"

#ifdef __cplusplus
}
#endif

/**
 * @brief Creates a new instance of an IDictionary implementation.
 *
 * Allocates and initializes a new dictionary object.
 *
 * @return Pointer to the newly created IDictionary instance.
 *         Returns NULL if allocation fails.
 */
struct IDictionary *collection_dictionary_new();

/**
 * @brief Frees an IDictionary instance and releases its resources.
 *
 * After calling this function, the dictionary pointer is set to NULL.
 *
 * @param dictionary Double pointer to the IDictionary instance to free.
 *                   Must not be NULL.
 */
void collection_dictionary_free(struct IDictionary **dictionary);

#ifdef __cplusplus
}
#endif
