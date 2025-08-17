/**
 * @file iarray.h
 * @brief Interface definitions for a generic thread-safe singly linked list (IArray).
 *
 * Defines the IArray interface and the List node structure,
 * providing function pointers for list manipulation operations
 * such as push, pop, shift, unshift, contains, clone, and clear.
 *
 * Designed for concurrent access with thread-safety expected from implementations.
 */
#pragma once

#include <stdbool.h>

/**
 * @brief Interface for a thread-safe, generic, singly linked list.
 */
struct IArray {
    /**
     * @brief Inserts an item at the beginning of the list.
     * @param self Pointer to the IArray instance.
     * @param item Pointer to the item to insert.
     * @return The inserted item pointer on success, or NULL on failure.
     */
    const void *(*unshift)(struct IArray *self, const void *item);

    /**
     * @brief Appends an item at the end of the list.
     * @param self Pointer to the IArray instance.
     * @param item Pointer to the item to append.
     * @return The appended item pointer on success, or NULL on failure.
     */
    const void *(*push)(struct IArray *self, const void *item);

    /**
     * @brief Checks if the list contains a specific item.
     * @param self Pointer to the IArray instance.
     * @param item Pointer to the item to search for.
     * @return true if the item exists in the list, false otherwise.
     */
    bool (*containsValue)(const struct IArray *self, const void *item);

    /**
     * @brief Removes and returns the first item in the list.
     * @param self Pointer to the IArray instance.
     * @return The removed item pointer, or NULL if list is empty.
     */
    const void *(*shift)(struct IArray *self);

    /**
     * @brief Removes and returns the last item in the list.
     * @param self Pointer to the IArray instance.
     * @return The removed item pointer, or NULL if list is empty.
     */
    const void *(*pop)(struct IArray *self);

    /**
     * @brief Removes and returns a specific item from the list.
     * @param self Pointer to the IArray instance.
     * @param item Pointer to the item to remove.
     * @return The removed item pointer, or NULL if not found.
     */
    void *(*removeItem)(struct IArray *self, const void *item);

    /**
     * @brief Clones the list (shallow copy).
     * @param self Pointer to the IArray instance.
     * @return A new IArray instance that is a shallow copy of the original, or NULL on failure.
     */
    struct IArray *(*clone)(const struct IArray *self);

    /**
     * @brief Returns the number of items in the list.
     * @param self Pointer to the IArray instance.
     * @return Number of items in the list.
     */
    int (*size)(const struct IArray *self);

    /**
     * @brief Clears all items from the list.
     * @param self Pointer to the IArray instance.
     * @param callback Optional callback called on each item before it is freed. Can be NULL.
     */
    void (*clear)(struct IArray *self, void (*callback)(void *item));
};

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
