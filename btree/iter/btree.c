/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h
 * a připravených koster funkcí implementujte binární vyhledávací
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Solved by
 * Name: Albert Tikaiev
 * Login: xtikaia00
*/

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci.
 */
void bst_init(bst_node_t **tree)
{
  (*tree) = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * ukazatel na obsah daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value)
{
  while(tree != NULL){
    //Found node with the same key
    if(tree->key == key)
    {
      (*value) = &tree->content;
      return true;
    }
    //Key is lesser than key in tree
    //Continue searching in left subtree
    else if (key < tree->key) tree = tree->left; 
    //Key is bigger than key in tree
    //Continue searching in right subtree
    else tree = tree->right;
  }
  //tree == NULL
  //That means there is no node with the same key
  return false;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, bst_node_content_t value)
{
  while (true) 
  {
    //There wasn't node with the same key
    //That means it has to be created
    if((*tree) == NULL)
    {
      //Allocate new node and check malloc
      (*tree) = malloc(sizeof(bst_node_t));
      if((*tree) == NULL) return;
      //Load data in new node
      (*tree)->content = value;
      (*tree)->key = key;
      //New node doesn't have any sons
      (*tree)->right=NULL;
      (*tree)->left=NULL;
      return;
    }
    //Otherwise
    else
    {
      //Key is lesser than key in tree
      //Continue searching in left subtree
      if(key < (*tree)->key) tree = &(*tree)->left;
      //Key is bigger than key in tree
      //Continue searching in right subtree
      else if(key>(*tree)->key) tree = &(*tree)->right;
      //Found node with the same key
      else
      {
        //Deallocate old value
        free((*tree)->content.value);
        //Insert new value
        (*tree)->content.value = value.value;
        (*tree)->content.type = value.type;
        return;
      }
    }
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  while(true){
    //Found most right node in left subtree for target
    if((*tree)->right == NULL)
    {
      //New key
      target->key = (*tree)->key;
      //New content
      free(target->content.value);
      target->content = (*tree)->content;
      //Deallocate most right node and set it to NULL
      free((*tree));
      (*tree) = NULL;
      return;
    }
    //Continue searching
    tree = &(*tree)->right;
  }
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 *
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key)
{
  while(true){
    //Node wasn't found -> fuction is doing nothing
    if((*tree)==NULL) return;
    //Key is lesser than key in tree
    //Continue searching in left subtree
    if(key < (*tree)->key) tree = &(*tree)->left;
    //Key is bigger than key in tree
    //Continue searching in right subtree
    else if(key > (*tree)->key) tree = &(*tree)->right;
    //Found node with the same key
    else
    {
      //If this node has two sons
      if((*tree)->right != NULL && (*tree)->left != NULL)
      {
        //Replace this node with most rigth node in its left subtree
        bst_replace_by_rightmost((*tree), &(*tree)->left);
      }
      //Otherwise
      else 
      {
        //If node is leaf
        //Then it will be NULL at the end
        bst_node_t *temp = NULL;
        //Otherwise it will be replaced with one of his existing sons
        if((*tree)->right != NULL) temp = (*tree)->right;
        else if ((*tree)->left != NULL) temp = (*tree)->left;
        //Deallocate old node
        free((*tree)->content.value);
        free((*tree));
        //Insert new node on this position or NULL
        (*tree) = temp;
      }
      return;
    }
  }
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree)
{
  //If tree is empty function does nothing
  if((*tree) == NULL) return;
  //Allocate stack for disposing and check malloc
  stack_bst_t *dstack = malloc(sizeof(stack_bst_t));
  if(dstack == NULL) return;
  stack_bst_init(dstack);
  //Push the root
  stack_bst_push(dstack, (*tree));
  
  while(!stack_bst_empty(dstack))
  {
    //Pop node that has to be disposed
    bst_node_t *temp = stack_bst_pop(dstack);
    //Add to stack sons that have to be disposed
    //If they aren't already NULL
    if(temp->left != NULL) stack_bst_push(dstack, temp->left);
    if(temp->right != NULL) stack_bst_push(dstack, temp->right);
    //Deallocate node
    free(temp->content.value);
    free(temp);
    //NULL disposed node
    temp = NULL;
  }
  (*tree) = NULL;
  free(dstack);
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items)
{
  //Add all roots to items
  //Iterate through all left nodes
  while(tree != NULL)
  {
    bst_add_node_to_items(tree, items);
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
  // ROOT -> LEFT -> RIGHT
  //Allocate stack for preorder and check malloc
  stack_bst_t *prestack = malloc(sizeof(stack_bst_t));
  if(prestack == NULL) return;
  stack_bst_init(prestack);
  //Call leftmost on root
  bst_leftmost_preorder(tree, prestack, items);
  //Call leftmost on all other nodes
  while(!stack_bst_empty(prestack))
  {
    //Take already processed node from stack
    bst_node_t *temp = stack_bst_pop(prestack);
    //Call it on rigth nodes of already processed nodes from stack 
    bst_leftmost_preorder(temp->right, prestack, items);
  }
  free(prestack);
}

/*
 * Pomocná funkce pro iterativní inorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{
  //Just iterate through all left nodes and add them to stack
  while(tree != NULL)
  {
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
  // LEFT -> ROOT -> RIGHT
  //Allocate stack for inorder and check malloc
  stack_bst_t *instack = malloc(sizeof(stack_bst_t));
  if(instack == NULL) return;
  stack_bst_init(instack);
  //Call leftmost on root
  bst_leftmost_inorder(tree, instack);
  //Call leftmost on all other nodes
  while(!stack_bst_empty(instack))
  {
    //Take most left node, that wasn't processed yet 
    bst_node_t *temp = stack_bst_pop(instack);
    //Add it to items
    bst_add_node_to_items(temp, items);
    //Process and call leftmost on this node
    bst_leftmost_inorder(temp->right, instack);
  }
  free(instack);
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit)
{
  //Push nodes to stacks and its first visiting value
  //Iterate through all left nodes
  while(tree != NULL)
  {
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
    tree = tree->left;
  }

}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
  // LEFT -> RIGHT -> ROOT
  //Allocate stacks for postorder and check mallocs
  stack_bst_t *poststack_visit = malloc(sizeof(stack_bst_t));
  if(poststack_visit == NULL) return;
  stack_bst_init(poststack_visit);
  stack_bool_t *poststack_first = malloc(sizeof(stack_bool_t));
  if(poststack_first == NULL) return;
  stack_bool_init(poststack_first);
  //Call leftmost on root
  bst_leftmost_postorder(tree, poststack_visit, poststack_first);
  //Call leftmost on all other nodes
  while(!stack_bst_empty(poststack_visit))
  {
    //Take from stacks node and its first visiting value
    bst_node_t *nodetemp = stack_bst_top(poststack_visit);
    bool booltemp = stack_bool_pop(poststack_first);
    //If its value of first visit is true
    //Push this node back to stack with first visit value of False
    if(booltemp)
    {
      stack_bool_push(poststack_first, false);
      //And call leftmost on its right node
      bst_leftmost_postorder(nodetemp->right, poststack_visit, poststack_first);
    }
    //This node was already visited
    //Now it can be added to the items
    else 
    {
      //Pop this node and add to the items
      stack_bst_pop(poststack_visit);
      bst_add_node_to_items(nodetemp, items);
    }
  }
  free(poststack_first);
  free(poststack_visit);
}
