# IAL 1. homework
The goal of this homework is to practically use knowledge to implement the following data structures:
- `hashtable` Hashtable
- `btree/iter` Binary tree, iterative implementation
- `btree/rec` Binary tree, recursive implementation
- `btree/exa` Practical application of Binary tree

### Evaluation 10/10

# Hashtable
In this part of the homework, separate chaining was used to resolve collisions.
- `ht_init` Initialize table
- `ht_search` Search for element in table
- `ht_insert` Insert element in table
- `ht_get` Get value of element from table
- `ht_delete` Remove element from table
- `ht_delete_all` Remove all elements from table

# Binary tree
The same methods have been developed in recursive and iterative implementations.
- `bst_init` Initialize tree
- `bst_search` Search for element in tree
- `bst_insert` Insert element in tree
- `bst_replace_by_rightmost` Auxiliary function for removing element, which replaces a node with its rightmost child
- `bst_delete` Remove element from tree
- `bst_dispose` Free all nodes in tree
- `bst_preorder` Performing preorder traversal
- `bst_inorder` Performing inorder traversal
- `bst_postorder` Performing postorder traversal

### Practical application
This part of the homework uses a binary tree to count certain characters in strings as follows:

- `Letters [A-Z-Z]` will have separate nodes for counting (case insensetive)
- `Space ' '` will have a separate node for counting
- `Other characters` will be counted into the node with the key `'_'`
