/*!
 * @file bst.c
 * @brief functions related to managing a binary search tree, primarily
 * used by @ref restore_select()
 */
/*
 *
 * Most of the code used here was adapted from the tutorials at:
 * <http://www.zentut.com/c-tutorial/c-binary-search-tree/>
 * <https://www.geeksforgeeks.org/tree-traversals-inorder-preorder-and-postorder/>
 *
 */

#include "bst.h"
#include "messages_rmw.h"

/*!
 * create a new node
 * @param[in] data
 * @param[in] size_str The formatted string that shows the file size when @ref restore_select() is used
 * @return node
 * @see dispose()
 */
static st_node *
create_node (char *data, char *size_str)
{
  st_node *new_node = (st_node *) malloc (sizeof (st_node));
  chk_malloc (new_node, __func__, __LINE__);
  new_node->data = (char *) calloc (strlen (data) + 1, 1);
  chk_malloc (new_node->data, __func__, __LINE__);

  strcpy (new_node->data, data);
  new_node->size_str = size_str;
  new_node->left = NULL;
  new_node->right = NULL;
  return new_node;
}

/*!
 * Insert a new node into the binary search tree; used in
 * @ref restore_select()
 * @param[out] root
 * @see create_node
 */
st_node *
insert_node (st_node * root, comparer strcasecmp, char *data, char *size_str)
{

  if (root == NULL)
  {
    root = create_node (data, size_str);
  }
  else
  {
    int is_left = 0;
    int r = 0;
    st_node *cursor = root;
    st_node *prev = NULL;

    while (cursor != NULL)
    {
      r = strcasecmp (data, cursor->data);
      prev = cursor;
      /* creating the tree will hang if 0 isn't accounted for.
       * An example of an equal string would be "Makefile" and "makefile"
       * Two separate files but because we're using strcasecmp, the tree is sorted
       * without case sensitivity
       */
      if (r <= 0)
      {
        is_left = 1;
        cursor = cursor->left;
      }
      else
      {
        is_left = 0;
        cursor = cursor->right;
      }
    }
    if (is_left)
      prev->left = create_node (data, size_str);
    else
      prev->right = create_node (data, size_str);

  }
  return root;
}

/*!
 * Add a binary search tree to an array to populate the list of menu items
 * @param[in] node
 * @param[out] my_items
 * @param[out] level_one
 * @see restore_select()
 */
void
populate_menu (st_node * node, ITEM ** my_items, bool level_one)
{
  static int i;
  if (level_one)
    i = 0;

  if (node == NULL)
    return;

  /* first recur on left child */
  populate_menu (node->left, my_items, false);

  my_items[i] = new_item (node->data, node->size_str);
  i++;

  /* now recur on right child */
  populate_menu (node->right, my_items, false);
}

/*!
 * recursively remove all nodes of the tree
 * @param[out] root
 * @return void
 * @see create_node()
 */
void
dispose (st_node * root)
{
  if (root != NULL)
  {
    dispose (root->left);
    dispose (root->right);
    free (root->data);
    free (root->size_str);

    free (root);
  }
}
