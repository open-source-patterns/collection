/**
 * @file i_dictionary.h
 * @ingroup Collection
 * @brief Dictionary Interface
 *
 * This header declares the IDictionary struct, which provides
 * an abstract interface for managing string-keyed collections
 * of arbitrary pointer values. It supports typical dictionary
 * operations such as lookup, insertion, removal, and clearing.
 *
 * Implementations of this interface can vary in storage and
 * performance characteristics but share this common API.
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#pragma once

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Interface for a generic key-value dictionary.
 *
 * Stores associations between string keys and arbitrary pointer values.
 */
struct IDictionary {
    /**
     * @brief Retrieves the value associated with the given key.
     *
     * @param self Pointer to the dictionary instance.
     * @param key Null-terminated string key to look up.
     * @return true if the item gets replaced from the list, false otherwise.
     */
    void *(*get)(const struct IDictionary *self, const char *key);

    /**
     * @brief Adds or updates a key-value pair in the dictionary.
     *
     * @param self Pointer to the dictionary instance.
     * @param key Null-terminated string key to add or update.
     * @param value Pointer to the value to associate with the key.
     * @return true if insertion or update was successful; false on failure (e.g. memory allocation failure).
     */
    bool (*put)(struct IDictionary *self, const char *key, const void *value);

    /**
     * @brief Checks whether the dictionary contains a given key.
     *
     * @param self Pointer to the dictionary instance.
     * @param key Null-terminated string key to check.
     * @return true if the key exists; false otherwise.
     */
    bool (*contains_key)(const struct IDictionary *self, const char *key);

    /**
     * @brief Removes the key-value pair associated with the given key.
     *
     * @param self Pointer to the dictionary instance.
     * @param key Null-terminated string key to remove.
     * @return Pointer to the removed value if the key was found; otherwise NULL.
     *         Caller is responsible for managing the memory of the returned value.
     */
    void *(*remove_item)(struct IDictionary *self, const char *key);

    /**
     * @brief Replaces the value associated with a key, returning the previous value.
     *
     * @param self Pointer to the dictionary instance.
     * @param key Null-terminated string key to update.
     * @param value New value pointer to associate with the key.
     * @return Pointer to the old value if key was found; otherwise NULL.
     *         Caller is responsible for managing the memory of the returned value.
     */
    void *(*replace)(const struct IDictionary *self, const char *key, const void *value);

    /**
     * @brief Clears all key-value pairs from the dictionary.
     *
     * @param self Pointer to the dictionary instance.
     * @param destructor Optional function pointer called for each value before removal.
     *                 May be NULL if no cleanup is needed.
     * @return true if the list gets cleared, false otherwise.
     */
    bool (*clear)(const struct IDictionary *self, void (*destructor)(void *value));
};

struct IDictionary *collection_dictionary_new(void);

/**
 * @brief Deinitializes an Dictionary instance and releases internal resources.
 *
 * @param dictionary Dictionary instance to deinitialize.
 * @param destructor Optional callback used to destroy stored item values.
 */
void collection_dictionary_dealloc(struct IDictionary **dictionary, void (*destructor)(void *item));
