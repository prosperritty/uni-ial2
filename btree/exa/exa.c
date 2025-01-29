/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte triviální funkci letter_count. Všimněte si, že výstupní strom může být značně degradovaný 
 * (až na úroveň lineárního seznamu). Jako typ hodnoty v uzlu stromu využijte 'INTEGER'.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Solved by
 * Name: Albert Tikaiev
 * Login: xtikaia00
*/

/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) {
  //Insert value will be always type of INTEGER
  bst_node_content_t insert_value;
  insert_value.type = INTEGER;
  bst_node_content_t *value = NULL;
  int *newval = NULL;

  for(int i = 0; input[i] != '\0' ;i++){
    char currinput;
    newval = malloc(sizeof(int));
    //Case insensitive letters a-Z
    if(input[i] < 91 && input[i] > 64) currinput = input[i] + 32;
    else if (input[i] < 123 && input[i] > 96) currinput = input[i];
    //White space
    else if (input[i] == ' ') currinput = ' ';
    //Other inputs
    else currinput = '_';
    
    //If there is node with the same key
    //Just increase its content value by one
    if(bst_search((*tree), currinput, &value)){
      *newval = *((int*)value->value) + 1;
    }
    //Otherwise set content value to 1
    else{
      *newval = 1;
    }
    
    //And insert new content with new pointer
    insert_value.value = newval;
    bst_insert(tree, currinput, insert_value);
    //NULL already inserted pointer
    newval = NULL;
  }
}
