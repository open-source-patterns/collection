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

#include "collection/Mutex.h"
#include "collection/IArray.h"

/**
 * @struct ArrayNode
 * @brief Represents a ArrayNode in a singly linked list.
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
 */
struct Array {
    struct IArray base;             /**< 🧩 Base interface for array operations */
    struct ArrayNode *list;         /**< 🗃️ Head of the singly linked ArrayNode */
    Mutex mutex;                   /**< 🔒 Mutex for thread safety */
};
