/**
 * @file i_array.h
 * @ingroup Collection
 * @brief Array Interface
 *
 * Defines the IArray interface and the List node structure,
 * providing function pointers for list manipulation operations
 * such as push, pop, shift, unshift, contains, clone, and clear.
 *
 * Designed for concurrent access with thread-safety expected from implementations.
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#pragma once

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Interface for a thread-safe, generic, singly linked list.
 */
struct IArray {
    /**
     * @brief Returns item at index.
     * @return Item pointer, or NULL if index is out of range.
     */
    const void *(*get)(const struct IArray *self, size_t index);

    void *(*put)(struct IArray *self, const void *item, size_t index);

    /**
     * @brief Iterates over each item in the list.
     * @param self Pointer to the IArray instance.
     * @param consumer Function called for each item. Receives the element, data and out-param error.
     * @param data Optional data passed to consumer Function (e.g., notification).
     */
    void (*for_each)(const struct IArray *self, void (*consumer)(const void *element, const void *data), const void *data);

    /**
     * @brief Finds the first element in the array that matches the given predicate.
     * @param self Pointer to the IArray instance.
     * @param predicate Function that returns true for the desired element.
     * @param data Optional user data passed to the predicate function.
     * @return Pointer to the first matching element, or NULL if no match is found.
     */
    const void *(*find)(const struct IArray *self, bool (*predicate)(const void *element, const void *data), const void *data);

    size_t (*first_index)(const struct IArray *self, bool (*predicate)(const void *element, const void *data), const void *data);

    size_t (*last_index)(const struct IArray *self, bool (*predicate)(const void *element, const void *data), const void *data);

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
     * @return true if the item gets inserted in the list, false otherwise.
     */
    bool (*push)(struct IArray *self, const void *item);

    /**
     * @brief Checks if the list contains a specific item.
     * @param self Pointer to the IArray instance.
     * @param item Pointer to the item to search for.
     * @return true if the item exists in the list, false otherwise.
     */
    bool (*contains_value)(const struct IArray *self, const void *item);

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
    void *(*remove_item)(struct IArray *self, const void *item);

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
    size_t (*count)(const struct IArray *self);

    /**
     * @brief Clears all items from the list.
     * @param self Pointer to the IArray instance.
     * @param destructor Optional destructor called on each item to free it. Can be NULL.
     */
    void (*clear)(struct IArray *self, void (*destructor)(void *item));
};

struct IArray *collection_array_new(void);

/**
 * @brief Deinitializes an Array instance and releases internal resources.
 *
 * @param array Array instance to deinitialize.
 * @param destructor Optional callback used to destroy stored item values.
 */
void collection_array_dealloc(struct IArray **array, void (*destructor)(void *item));
