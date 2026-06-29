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
 * @brief Represents a node in a singly linked list.
 *
 * Each ArrayNode stores a generic pointer to an item and a pointer to the next ArrayNode in the list.
 */
struct ArrayNode {
    const void *item;               /**< 📦 Pointer to the stored item (generic type) */
    struct ArrayNode *next;         /**< ➡️ Pointer to the next ArrayNode in the singly linked list */
};

/**
 * @struct Array
 * @brief Represents a linked-list-based array structure.
 *
 * Contains a base interface (IArray) and a pointer to the head ArrayNode of the singly linked list.
 *
 * Defines the Array struct that implements the IArray interface,
 * along with allocation, initialization, creation, and free functions.
 *
 * This implementation provides thread-safe list operations.
 */
struct Array {
    struct IArray super;            /**< 🧩 Base interface for array operations */
    struct ArrayNode *list;         /**< 🗃️ Head of the singly linked ArrayNode */
    Mutex mutex;                    /**< 🔒 Mutex for thread safety */
};
