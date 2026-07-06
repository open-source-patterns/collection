/**
 * @file i_array.h
 * @ingroup Collection
 * @brief Array Interface
 *
 * Defines the IArray interface for generic container implementations,
 * providing function pointers for common operations such as insertion,
 * removal, lookup, iteration, cloning, and clearing.
 *
 * Implementations of this interface are expected to provide thread-safe
 * operations.
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#pragma once

#include <stdbool.h>
#include <stddef.h>

#pragma region Types

/**
 * @brief Interface for a generic, thread-safe array.
 */
struct IArray {
    /**
     * @brief Returns the element at the specified index.
     *
     * @param self Pointer to the array instance.
     * @param index Zero-based index.
     *
     * @return Pointer to the element, or NULL if the index is out of range.
     */
    const void *(*get)(const struct IArray *self, size_t index);

    /**
     * @brief Invokes a callback for each element in the array.
     *
     * @param self Pointer to the array instance.
     * @param consumer Callback invoked for each element.
     * @param data Optional user data passed to the callback.
     */
    void (*for_each)(const struct IArray *self, void (*consumer)(const void *element, const void *data), const void *data);

    /**
     * @brief Finds the first element matching a predicate.
     *
     * @param self Pointer to the array instance.
     * @param predicate Predicate invoked for each element.
     * @param data Optional user data passed to the predicate.
     *
     * @return Pointer to the first matching element, or NULL if no match is found.
     */
    const void *(*find)(const struct IArray *self, bool (*predicate)(const void *element, const void *data), const void *data);

    /**
     * @brief Returns the index of the first matching element.
     *
     * @param self Pointer to the array instance.
     * @param predicate Predicate invoked for each element.
     * @param data Optional user data passed to the predicate.
     *
     * @return Zero-based index of the first matching element, or SIZE_MAX if no match is found.
     */
    size_t (*first_index)(const struct IArray *self, bool (*predicate)(const void *element, const void *data), const void *data);

    /**
     * @brief Returns the index of the last matching element.
     *
     * @param self Pointer to the array instance.
     * @param predicate Predicate invoked for each element.
     * @param data Optional user data passed to the predicate.
     *
     * @return Zero-based index of the last matching element, or SIZE_MAX if no match is found.
     */
    size_t (*last_index)(const struct IArray *self, bool (*predicate)(const void *element, const void *data), const void *data);

    /**
     * @brief Determines whether the array contains the specified element.
     *
     * @param self Pointer to the array instance.
     * @param item Pointer to the element to search for.
     *
     * @return true if the element exists; otherwise false.
     */
    bool (*contains_value)(const struct IArray *self, const void *item);

    /**
     * @brief Returns the number of elements in the array.
     *
     * @param self Pointer to the array instance.
     *
     * @return Number of elements in the array.
     */
    size_t (*count)(const struct IArray *self);

    /**
     * @brief Creates a shallow copy of the array.
     *
     * @param self Pointer to the array instance.
     *
     * @return Newly allocated array, or NULL on failure.
     */
    struct IArray *(*clone)(const struct IArray *self);

    /**
     * @brief Replaces the element at the specified index.
     *
     * @param self Pointer to the array instance.
     * @param item Pointer to the replacement element.
     * @param index Zero-based index.
     *
     * @return Pointer to the previous element, or NULL if the index is out of range.
     */
    void *(*put)(struct IArray *self, const void *item, size_t index);

    /**
     * @brief Inserts an element at the beginning of the array.
     *
     * @param self Pointer to the array instance.
     * @param item Pointer to the element to insert.
     *
     * @return Pointer to the inserted element, or NULL on failure.
     */
    const void *(*unshift)(struct IArray *self, const void *item);

    /**
     * @brief Appends an element to the end of the array.
     *
     * @param self Pointer to the array instance.
     * @param item Pointer to the element to append.
     *
     * @return true if the element was inserted successfully; otherwise false.
     */
    bool (*push)(struct IArray *self, const void *item);

    /**
     * @brief Removes and returns the first element.
     *
     * @param self Pointer to the array instance.
     *
     * @return Pointer to the removed element, or NULL if the array is empty.
     */
    const void *(*shift)(struct IArray *self);

    /**
     * @brief Removes and returns the last element.
     *
     * @param self Pointer to the array instance.
     *
     * @return Pointer to the removed element, or NULL if the array is empty.
     */
    const void *(*pop)(struct IArray *self);

    /**
     * @brief Removes the specified element from the array.
     *
     * @param self Pointer to the array instance.
     * @param item Pointer to the element to remove.
     *
     * @return Pointer to the removed element, or NULL if the element was not found.
     */
    void *(*remove_item)(struct IArray *self, const void *item);

    /**
     * @brief Removes all elements from the array.
     *
     * @param self self Pointer to the array instance.
     * @param destructor Optional destructor called on each item before removal. Can be NULL.
     */
    void (*clear)(struct IArray *self, void (*destructor)(void *item));
};

#pragma endregion

#pragma region Public API

/**
 * @brief Creates a new array instance.
 *
 * @return A newly allocated array, or NULL if allocation fails.
 */
struct IArray *collection_array_new(void);

/**
 * @brief Destroys an array instance.
 *
 * Releases all internal resources and optionally destroys each stored
 * element using the supplied destructor.
 *
 * @param array Pointer to the array pointer. On successful return, *array is set to NULL.
 * @param destructor destructor Optional callback invoked for each stored element before removal. May be NULL.
 */
void collection_array_dealloc(struct IArray **array, void (*destructor)(void *item));

#pragma endregion
