/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
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
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value)
{
  //Hasn't found
  if(tree == NULL) return false;
  //Was found
  if(tree->key == key)
  {
    (*value) = &tree->content;
    return true;
  }
  //Key is lesser than key in tree
  //Continue searching in left subtree
  else if(key < tree->key) 
  {
    return bst_search(tree->left, key, value);
  }
  //Key is bigger than key in tree
  //Continue searching in right subtree
  else 
  {
    return bst_search(tree->right, key, value);
  }
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
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, bst_node_content_t value)
{
  //There wasn't node with the same key
  //That means it has to be created
  if((*tree) == NULL)
  {
    //Allocate new node and check malloc
    (*tree) = malloc(sizeof(bst_node_t));
    if((*tree) == NULL) return;
    //Load data in new node
    (*tree)->key = key;
    (*tree)->content = value;
    //New node doesn't have any sons
    (*tree)->left = NULL;
    (*tree)->right = NULL;
    
  }
  //Continue to search for node with the same key
  else{
    //Key is lesser than key in tree
    //Continue searching in left subtree
    if(key < (*tree)->key) bst_insert(&((*tree)->left), key, value);
    //Key is bigger than key in tree
    //Continue searching in right subtree
    else if(key > (*tree)->key) bst_insert(&((*tree)->right), key, value);
    //Found node with the same key
    else
    {
      //Deallocate old value
      free((*tree)->content.value);
      //Insert new value
      (*tree)->content.type = value.type;
      (*tree)->content.value = value.value;
      return;
    }
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
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
    (*tree)=NULL;
  }
  //Continue searching
  else bst_replace_by_rightmost(target, &(*tree)->right);
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
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key)
{
  //Node wasn't found -> fuction is doing nothing
  if((*tree) != NULL)
  {
    //Key is lesser than key in tree
    //Continue searching in left subtree
    if(key < (*tree)->key) bst_delete(&((*tree)->left), key);
    //Key is bigger than key in tree
    //Continue searching in right subtree
    else if(key > (*tree)->key) bst_delete(&((*tree)->right), key);
    //Found node with the same key
    else 
    {
      //If this node has two sons
      if((*tree)->left != NULL && (*tree)->right != NULL)
      {
        //Replace this node with most rigth node in its left subtree
        bst_replace_by_rightmost((*tree), &(*tree)->left);
      }
      //Otherwise
      else 
      {
        //If node is leaf
        //Then it will be NULL at the end
        bst_node_t* temp = NULL;
        //Otherwise it will be replaced with one of his existing sons
        if((*tree)->right != NULL) temp = (*tree)->right;
        else if ((*tree)->left!= NULL) temp = (*tree)->left;
        //Deallocate old node
        free((*tree)->content.value);
        free((*tree));
        //Insert new node on this position or NULL
        (*tree) = temp;
      }
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
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree)
{
  //If this node is not NULL
  //Firstly try to dispose his sons
  //Then dispose this node
  if((*tree) != NULL)
  {
    bst_dispose(&((*tree)->left));
    bst_dispose(&((*tree)->right));
    //Firstly node content, then node
    free((*tree)->content.value);
    free((*tree));
    (*tree)=NULL;
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
  // ROOT -> LEFT -> RIGHT
  if(tree != NULL){
  bst_add_node_to_items(tree, items);
  bst_preorder(tree->left, items);
  bst_preorder(tree->right, items);
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
  // LEFT -> ROOT -> RIGHT
  if(tree != NULL){
  bst_inorder(tree->left, items);
  bst_add_node_to_items(tree, items);
  bst_inorder(tree->right, items);
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
  //LEFT -> RIGHT -> ROOT
  if(tree != NULL){
  bst_postorder(tree->left, items);
  bst_postorder(tree->right, items);
  bst_add_node_to_items(tree, items);
  }
}
