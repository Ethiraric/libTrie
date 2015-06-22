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
#include <stdlib.h>
#include <string.h>
#include "trie.h"

/*
** The words to insert in sort tests
** They need not be in alphabetical order
*/
const char *sort_words[] =
{
  "coucou",
  "blop",
  "toto",
  "foo",
  "bar",
  "cou"
};

/*
** Number of words in sort_words
*/
#define SORT_NB_WORDS	(sizeof(sort_words) / sizeof(*sort_words))

/*
** Comparison function for words
*/
int		sort_words_cmp(const void *vlhs, const void *vrhs)
{
  const char * const *lhs = (const char * const *)(vlhs);
  const char * const *rhs = (const char * const *)(vrhs);

  return (strcmp(*lhs, *rhs));
}

/*
** Comparison function for words for postorder traversal
*/
int		sort_words_postorder_cmp(const void *vlhs, const void *vrhs)
{
  const char * const *lhs = (const char * const *)(vlhs);
  const char * const *rhs = (const char * const *)(vrhs);
  size_t	lenl = strlen(*lhs);
  size_t	lenr = strlen(*rhs);
  int		tmp;

  if (lenl < lenr)
    {
      tmp = strncmp(*lhs, *rhs, lenl);
      if (!tmp)
	return (1);
      return (tmp);
    }
  if (lenl > lenr)
    {
      tmp = strncmp(*lhs, *rhs, lenr);
      if (!tmp)
	return (-1);
      return (tmp);
    }
  return (strcmp(*lhs, *rhs));
}

/*
** The recursive function of dump_trie
** Displays the key if the node has the marker set
*/
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

/*
** Displays all keys in a trie
*/
void		dump_trie(t_trie *trie)
{
  static char	buff[BUFSIZ] = {0};

  dump_node(trie, buff, buff);
}

int		assert_traversal_test_sorted(const char *key,
					     void *value, void *dat)
{
  static int	count = 0;

  (void)(value);
  (void)(dat);
  assert(!strcmp(key, sort_words[count]));
  ++count;
  return (0);
}

int		assert_traversal_test_posto(const char *key,
					    void *value, void *dat)
{
  static int	count = 0;

  (void)(value);
  (void)(dat);
  assert(!strcmp(key, sort_words[count]));
  ++count;
  return (0);
}

/*
** Testing some insert, delete, key_exists, get_value
*/
void		run_basic_tests()
{
  t_trie	base;
  t_trie	*trie;

  trie = &base;
  trie_init(trie);

  /* Testing insertion */
  assert(!trie_insert(trie, "coucou", trie));
  assert(trie_get_value(trie, "coucou") == trie);
  assert(trie_key_exists(trie, "coucou"));
  assert(!trie_insert(trie, "cou", &trie));
  assert(trie_key_exists(trie, "cou"));
  assert(trie_get_value(trie, "cou") == &trie);

  /* Testing with non-existing keys */
  assert(!trie_key_exists(trie, "blop"));
  assert(trie_get_value(trie, "ofijwr") == NULL);
  assert(trie_get_value(trie, "co") == NULL);
  assert(trie_delete_key(trie, "co"));
  assert(trie_delete_key(trie, "ofijwr"));

  /* Testing more insertions */
  assert(!trie_insert(trie, "blop", &dump_node));
  assert(trie_key_exists(trie, "blop"));
  assert(trie_get_value(trie, "blop") == &dump_node);
  assert(!trie_insert(trie, "couille", &trie_init));
  assert(trie_key_exists(trie, "couille"));
  assert(trie_get_value(trie, "couille") == &trie_init);

  /* Testing deletion */
  assert(!trie_delete_key(trie, "coucou"));
  assert(!trie_key_exists(trie, "coucou"));
  assert(trie_get_value(trie, "coucou") == NULL);

  /* Testing annoying words (in the middle of other words) */
  assert(!trie_insert(trie, "cou", &trie));
  assert(trie_key_exists(trie, "cou"));
  assert(trie_get_value(trie, "cou") == &trie);

  /* Testing deletion of annoying word (must remove unused nodes) */
  assert(!trie_delete_key(trie, "couille"));
  assert(!trie_key_exists(trie, "couille"));
  assert(trie_get_value(trie, "couille") == NULL);

  trie_delete(trie);
}

/*
** Testing sort and traversals
*/
void		run_sort_traversal_tests()
{
  unsigned int	i;
  t_trie	base;
  t_trie	*trie;

  trie = &base;
  trie_init(trie);

  /* Filling trie */
  for (i = 0 ; i < SORT_NB_WORDS ; ++i)
    assert(!trie_insert(trie, sort_words[i], NULL));
  trie_sort(trie);
  qsort(sort_words, SORT_NB_WORDS, sizeof(char *), &sort_words_cmp);
  assert(!trie_traversal_preorder(trie, &assert_traversal_test_sorted, NULL));
  qsort(sort_words, SORT_NB_WORDS, sizeof(char *), &sort_words_postorder_cmp);
  assert(!trie_traversal_postorder(trie, &assert_traversal_test_posto, NULL));

  trie_delete(trie);
}

int		main()
{
  run_basic_tests();
  run_sort_traversal_tests();
  printf("All went well\n");
  return (0);
}
