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

void		dump_node(t_trie *trie, char *strcur, char *str)
{
  size_t	i;

  *strcur = trie->letter;
  if (trie->marker)
    printf("%s\n", str + 1); // First node is a '\0'
  for (i = 0 ; i < trie->nb_children ; ++i)
    dump_node(&trie->children[i], strcur + 1, str);
  *strcur = '\0';
}

void		dump_trie(t_trie *trie)
{
  static char	buff[BUFSIZ] = {0};

  dump_node(trie, buff, buff);
}

int		main()
{
  t_trie	base;
  t_trie	*trie;

  trie = &base;
  trie_init(trie);
  assert(!trie_insert(trie, "coucou", trie));
  assert(trie_get_value(trie, "coucou") == trie);
  assert(trie_key_exists(trie, "coucou"));

  assert(!trie_insert(trie, "cou", &trie));
  assert(trie_key_exists(trie, "cou"));
  assert(trie_get_value(trie, "cou") == &trie);

  assert(!trie_key_exists(trie, "blop"));
  assert(trie_get_value(trie, "ofijwr") == NULL);
  assert(trie_get_value(trie, "co") == NULL);
  assert(trie_delete_key(trie, "co"));
  assert(trie_delete_key(trie, "ofijwr"));

  assert(!trie_insert(trie, "blop", &main));
  assert(trie_key_exists(trie, "blop"));
  assert(trie_get_value(trie, "blop") == &main);

  assert(!trie_insert(trie, "couille", &trie_init));
  assert(trie_key_exists(trie, "couille"));
  assert(trie_get_value(trie, "couille") == &trie_init);

  assert(!trie_delete_key(trie, "coucou"));
  assert(!trie_key_exists(trie, "coucou"));
  assert(!trie_get_value(trie, "coucou"));

  assert(!trie_insert(trie, "cou", &trie));
  assert(trie_key_exists(trie, "cou"));
  assert(trie_get_value(trie, "cou") == &trie);

  assert(!trie_delete_key(trie, "couille"));
  assert(!trie_key_exists(trie, "couille"));
  assert(!trie_get_value(trie, "couille"));

  dump_trie(trie);

  trie_delete(trie);
  printf("All went well\n");
  return (0);
}
