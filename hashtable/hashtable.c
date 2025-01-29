/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Solved by
 * Name: Albert Tikaiev
 * Login: xtikaia00
*/

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
  for(int i = 0; i < HT_SIZE; i++){
    (*table)[i] = NULL;
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  int hash = get_hash(key);
  ht_item_t *item = (*table)[hash];
  //Iterate through synonyms
  while(item != NULL){
    //If key is the same stop iterating and return item
    if(item->key == key) break;
    item = item->next;
  }
  //NULL or item with the same key
  return item;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  int hash = get_hash(key);
  ht_item_t *item = ht_search(table, key);
  if(!item){
    //Alocate new item and check malloc
    ht_item_t* newitem = malloc(sizeof(ht_item_t));
    if(newitem == NULL) return;
    //Load data
    newitem->key = key;
    newitem->value = value;
    //Make first synonym of this hash in table to be next synonym of new item
    newitem->next = (*table)[hash];
    //Make new first synonym of this has in table to be new item
    (*table)[hash] = newitem;
  }
  //Otherwise just rewrite value
  else item->value = value;
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  ht_item_t *item = ht_search(table, key);
  if(item) return &(item->value);
  return NULL;
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  int hash = get_hash(key);
  //Take first synonym
  ht_item_t *item = (*table)[hash];
  //Variable for saving previous synonym
  ht_item_t *previtem = NULL;
  //Iterate through all synonyms
  while(item != NULL){
    //Synonym that has to be deleted was found
    if(item->key == key){
      //If it is first synonym
      //Make fisrt synonym to be next item of current first synonym
      if((*table)[hash] == item) (*table)[hash] = item->next;
      //Otherwise make previous next synonym to be next synonym of current synonym
      else previtem->next = item->next;
      //Deallocate current synonym and NULL it
      free(item);
      item = NULL;
      break;
    }
    //Make current synonym to be previous synonym
    previtem = item;
    //Take next synonym
    item = item->next;
  }
  //Function do nothing
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
  ht_item_t *item = NULL;
  ht_item_t *temp = NULL;
  //Iterate through all hashes
  for(int i = 0; i <HT_SIZE ;i++){
    //Take first synonym of certain hash
    item = (*table)[i];
    //Iterate through all synonyms of this hash
    while(item != NULL){
      //Take current synonym to temp and free it
      temp = item;
      //Take next synonym
      item = item->next;
      free(temp);
    }
    //Make value of this hash in table to be NULL
    (*table)[i] = NULL;
  }
}
