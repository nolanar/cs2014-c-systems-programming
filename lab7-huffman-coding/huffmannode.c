#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "huffmannode.h"

#define swap(x, y) \
{ \
    struct node *swap = (x); \
    (x) = (y); \
    (y) = swap; \
}

/**
 * Compares the weight of two nodes.
 *
 * Returns the difference of their weight:
 * return < 0 if this < that
 * return = 0 if this == that
 * return > 0 if this > that
 */
int compare(struct node *this, struct node *that);

struct node {
    int weight;
    int is_leaf;
    union {
        struct {
            struct node *left;
            struct node *right;
        } children;
        unsigned char ch;
    } value;
};

struct node *new_leaf_node(unsigned char ch, int weight)
{
    struct node *this = malloc(sizeof *this);
    this->weight = weight;
    this->value.ch = ch;
    this->is_leaf = 1;
    return this;
}

struct node *new_parent_node(struct node *left, struct node *right)
{
    struct node *this = malloc(sizeof *this);
    this->weight = left->weight + right->weight;
    this->value.children.left = left;
    this->value.children.right = right;
    this->is_leaf = 0;
    return this;
}

void free_node(struct node *this)
{
    free(this);
}

int is_leaf(struct node *this)
{
    return this->is_leaf;
}

int get_weight(struct node *this)
{
    return this->weight;
}

unsigned char get_char(struct node *this)
{
    assert(is_leaf(this));
    return this->value.ch;
} 

struct node *left_child(struct node *this)
{
    assert(!is_leaf(this));
    return this->value.children.left;
}

struct node *right_child(struct node *this)
{
    assert(!is_leaf(this));
    return this->value.children.right;
}

int compare(struct node *this, struct node *that) 
{
    return this->weight - that->weight;
}

/**
 * In-place quicksort of an array of nodes.
 */
void sort_nodes(struct node **nodes, int length)
{
    if (length < 2) {
        return;
    }
    if (length == 2) {
        /* swap if the only two nodes are in the wrong order */
        if (compare(nodes[0], nodes[1]) > 0) {
            swap(nodes[0], nodes[1]);
        }
        return;
    }
    /* use middle node as pivot */
    swap(nodes[length / 2], nodes[length - 1]);
    struct node *pivot = nodes[length - 1];
    /* find left and right nodes in wrong order */
    int left = 0;
    int right = length - 2;
    while (left <= right) {
        /* find rightmost node in wrong order to pivot */
        while(left <= right && compare(pivot, nodes[right]) <= 0) {
            right--;
        }
        /* find leftmost node in wrong order to pivot*/
        while (left <= right && compare(nodes[left], pivot) <= 0) {
            left++;
        }
        /* swap left and right if were found out of order */
        if (left <= right) {
            if (left != right) {
                swap(nodes[left], nodes[right]);
            }
            left++;
            right--;
        }
    }
    /* swap pivot and node at which left and right crossed */
    int swap_pos = right + 1;
    if (swap_pos != length - 1) {
        swap(nodes[swap_pos], nodes[length - 1])
    }
    /* call sort on left and right subarrays */
    sort_nodes(nodes, swap_pos);
    sort_nodes(nodes + (swap_pos + 1), length - (swap_pos + 1));
}

int binary_search_nodes(huff_node *nodes, int length, huff_node node)
{
    int value = get_weight(node);
    int lower = 0;
    int upper = length - 1;
    while (lower <= upper) {
        int mid = lower + (upper - lower) / 2;
        int mid_weight = get_weight(nodes[mid]);
        if (value == mid_weight) {
            return mid;
        }
        else if (value < mid_weight) {
            upper = mid - 1;
        }
        else if (value > mid_weight) {
            lower = mid + 1;
        }
    }
    return lower;
}