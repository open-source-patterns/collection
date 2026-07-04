/**
 * @file Array.h
 * @internal
 * @brief Array Header
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#pragma once

#include "collection/i_array.h"
#include "collection/i_platform.h"

/**
 * @struct ArrayNode
 * @brief Node used by the internal singly linked list.
 *
 * Stores one item pointer and a link to the next node.
 */
struct ArrayNode {
    const void *item;               /**< Stored item pointer. */
    struct ArrayNode *next;         /**< Next node in the list. */
};

/**
 * @struct Array
 * @brief Linked-list-backed implementation of IArray.
 *
 * Provides the internal storage for array operations using a singly linked
 * list protected by a mutex.
 */
struct Array {
    struct IArray super;            /**< IArray interface implemented by this type. */
    struct ArrayNode *list;         /**< Head node of the linked list. */
    Mutex mutex;                    /**< Mutex protecting list operations. */
};
