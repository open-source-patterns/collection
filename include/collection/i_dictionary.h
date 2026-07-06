/**
 * @file i_dictionary.h
 * @ingroup Collection
 * @brief Dictionary Interface
 *
 * Defines the IDictionary interface for generic key-value container
 * implementations, providing operations for lookup, insertion,
 * replacement, removal, and clearing.
 *
 * Implementations of this interface are expected to provide thread-safe
 * operations.
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#pragma once

#include <stdbool.h>

#pragma region Types

/**
 * @brief Interface for a generic, thread-safe dictionary.
 */
struct IDictionary {
    /**
     * @brief Returns the value associated with the specified key.
     *
     * @param self Pointer to the dictionary instance.
     * @param key Null-terminated key string.
     *
     * @return Pointer to the associated value, or NULL if the key is not found.
     */
    void *(*get)(const struct IDictionary *self, const char *key);

    /**
     * @brief Inserts or updates a key-value pair.
     *
     * @param self Pointer to the dictionary instance.
     * @param key Null-terminated key string.
     * @param value Pointer to the value to associate with the key.
     *
     * @return true if the operation succeeds; otherwise false.
     */
    bool (*put)(struct IDictionary *self, const char *key, const void *value);

    /**
     * @brief Determines whether the specified key exists.
     *
     * @param self Pointer to the dictionary instance.
     * @param key Null-terminated key string.
     *
     * @return true if the key exists; otherwise false.
     */
    bool (*contains_key)(const struct IDictionary *self, const char *key);

    /**
     * @brief Removes the specified key-value pair.
     *
     * @param self Pointer to the dictionary instance.
     * @param key Null-terminated key string.
     *
     * @return Pointer to the removed value, or NULL if the key is not found.
     */
    void *(*remove_item)(struct IDictionary *self, const char *key);

    /**
     * @brief Replaces the value associated with the specified key.
     *
     * @param self Pointer to the dictionary instance.
     * @param key Null-terminated key string.
     * @param value Replacement value.
     *
     * @return Pointer to the previous value, or NULL if the key is not found.
     */
    void *(*replace)(const struct IDictionary *self, const char *key, const void *value);

    /**
     * @brief Removes all key-value pairs from the dictionary.
     *
     * @param self Pointer to the dictionary instance.
     * @param destructor Optional callback invoked for each value before removal. May be NULL.
     *
     * @return true if the dictionary was cleared successfully; otherwise false.
     */
    bool (*clear)(const struct IDictionary *self, void (*destructor)(void *value));
};

#pragma endregion

#pragma region Public API

/**
 * @brief Creates a new dictionary instance.
 *
 * @return A newly allocated dictionary, or NULL if allocation fails.
 */
struct IDictionary *collection_dictionary_new(void);

/**
 * @brief Destroys a dictionary instance.
 *
 * Releases all internal resources and optionally destroys each stored
 * value using the supplied destructor.
 *
 * @param dictionary Pointer to the dictionary pointer. On successful return, *dictionary is set to NULL.
 * @param destructor Optional callback invoked for each stored value before destruction. May be NULL.
 */
void collection_dictionary_dealloc(struct IDictionary **dictionary, void (*destructor)(void *item));

#pragma endregion
