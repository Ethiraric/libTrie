/*
** main.c for  in /home/sabour_f/rendu/libTrie/tests
**
** Made by Florian SABOURIN
** Login   <sabour_f@epitech.net>
**
** Started on  Wed Jun 17 22:47:40 2015 Florian SABOURIN
** Last update Wed Jun 17 22:47:49 2015 Florian SABOURIN
*/

#include <assert.h>
#include <stdio.h>
#include "trie.h"

int		main()
{
  t_trie	base;
  t_trie	*trie;

  trie = &base;
  trie_init(trie);
  assert(!trie_insert(trie, "coucou", trie));
  assert(!trie_insert(trie, "cou", &trie));
  assert(trie_get_value(trie, "coucou") == trie);
  assert(trie_get_value(trie, "cou") == &trie);
  assert(trie_get_value(trie, "ofijwr") == NULL);
  assert(trie_key_exists(trie, "coucou"));
  assert(!trie_key_exists(trie, "blop"));
  assert(!trie_insert(trie, "blop", &main));
  assert(trie_key_exists(trie, "blop"));
  assert(trie_get_value(trie, "blop") == &main);
  assert(!trie_insert(trie, "couille", &trie_init));
  assert(trie_get_value(trie, "couille") == &trie_init);
  assert(trie_key_exists(trie, "couille"));
  trie_delete(trie);
  printf("All went well\n");
  return (0);
}
