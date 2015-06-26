/*
** trie.c for  in /home/sabour_f/rendu/libTrie/src
**
** Made by Florian SABOURIN
** Login   <sabour_f@epitech.net>
**
** Started on  Wed Jun 17 19:40:36 2015 Florian SABOURIN
** Last update Wed Jun 17 19:40:36 2015 Florian SABOURIN
*/

#include <errno.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "trie.h"

/*
** The step in allocation of the children member of t_trie
** A higher value may make insertions faster, but may need more memory
** This should never be more than 255 or less than 1
*/
#ifndef TRIE_ALLOC_STEP
# define TRIE_ALLOC_STEP	10
#endif

#if TRIE_ALLOC_STEP > 255
# warning "TRIE_ALLOC_STEP > 255 : setting to 255"
# undef TRIE_ALLOC_STEP
# define TRIE_ALLOC_STEP	255
#endif
#if TRIE_ALLOC_STEP < 1
# warning "TRIE_ALLOC_STEP < 1 : setting to 1"
# undef TRIE_ALLOC_STEP
# define TRIE_ALLOC_STEP	1
#endif

/*
** The step in allocation of the key for traversal functions
** A higher value may make the traversal faster, but may need more memory
** This should never be less than 1
*/
#ifndef TRAVERSAL_KEY_STEP
# define TRAVERSAL_KEY_STEP	1024
#endif

#if TRAVERSAL_KEY_STEP < 1
# warning "TRAVERSAL_KEY_STEP < 1 : setting to 1"
# undef TRAVERSAL_KEY_STEP
# define TRAVERSAL_KEY_STEP	1
#endif

/*
** Predicate to be passed to qsort to compare two nodes
** Nodes must be siblings
*/
static int	trie_qsort_predicate(const void *vlhs, const void *vrhs)
{
  t_trie	*lhs = (t_trie *)(vlhs);
  t_trie	*rhs = (t_trie *)(vrhs);

  return (lhs->letter - rhs->letter);
}

/*
** Reallocates the children member of trie so it can hold newsize t_trie
** Returns 0 on success, or a non-zero integer on failure
** Possible Errors :
** - ENOMEM : no more memory available
*/
static int	trie_realloc_children(t_trie *trie, size_t newsize)
{
  t_trie	*tmp;

  tmp = realloc(trie->children, newsize * sizeof(t_trie));
  if (!tmp)
    return (1);
  trie->children = tmp;
  trie->alloc_children = newsize;
  return (0);
}

/*
** Deletes a child from a trie
** Returns 0 on success, or a non-zero integer on failure
** Possible Errors :
** - child is not a child of trie
*/
static int	trie_delete_child(t_trie *trie, t_trie *child)
{
  unsigned int	nth;
  unsigned int	i;
  ptrdiff_t	diff;

  diff = (char *)(child) - (char *)(trie->children);
  if (diff < 0 || diff % sizeof(t_trie) ||
      (nth = diff / sizeof(t_trie)) > child->nb_children - 1)
    return (1);
  if (nth < trie->nb_children)
    memmove(child, child + 1, sizeof(t_trie) * (trie->nb_children - nth - 1));
  --trie->nb_children;
  for (; nth < trie->nb_children ; ++nth)
    for (i = 0 ; i < trie->children[nth].nb_children ; ++i)
      trie->children[nth].children[i].parent = trie->children + nth;
  return (0);
}

/*
** The recursion function for pre order traversal
** See trie_traversal_preorder for more informations
*/
int	trie_traversal_preorder_recur(
    t_trie *trie, int (*fct)(const char *, void *, void *), void *data,
    char **key, size_t depth, size_t keylen)
{
  size_t	i;
  char		*tmp;

  if (depth >= keylen)
    {
      keylen += TRAVERSAL_KEY_STEP;
      tmp = realloc(*key, keylen + 1);
      if (!tmp)
	return (1);
      *key = tmp;
      memset((*key) + depth, 0, keylen + 1 - depth);
    }
  (*key)[depth] = trie->letter;
  if (trie->marker)
    if (fct(*key, trie->value, data))
      return (1);
  for (i = 0 ; i < trie->nb_children ; ++i)
    if (trie_traversal_preorder_recur(&trie->children[i], fct, data, key,
				      depth + 1, keylen))
      return (1);
  (*key)[depth] = '\0';
  return (0);
}

/*
** The recursion function for post order traversal
** See trie_traversal_postorder for more informations
*/
int	trie_traversal_postorder_recur(
    t_trie *trie, int (*fct)(const char *, void *, void *), void *data,
    char **key, size_t depth, size_t keylen)
{
  size_t	i;
  char		*tmp;

  if (depth >= keylen)
    {
      keylen += TRAVERSAL_KEY_STEP;
      tmp = realloc(*key, keylen + 1);
      if (!tmp)
	return (1);
      *key = tmp;
      memset((*key) + depth, 0, keylen + 1 - depth);
    }
  (*key)[depth] = trie->letter;
  for (i = 0 ; i < trie->nb_children ; ++i)
    if (trie_traversal_postorder_recur(&trie->children[i], fct, data, key,
				       depth + 1, keylen))
      return (1);
  if (trie->marker)
    if (fct(*key, trie->value, data))
      return (1);
  (*key)[depth] = '\0';
  return (0);
}

/*
** Initializes a trie
*/
void		trie_init(t_trie *trie)
{
  trie->children = NULL;
  trie->letter = 0;
  trie->value = NULL;
  trie->nb_children = 0;
  trie->alloc_children = 0;
  trie->marker = false;
  trie->parent = NULL;
}

/*
** Deletes all resources allocated by the trie
** Does not free the trie pointer passed in parameter
*/
void		trie_delete(t_trie *trie)
{
  size_t	i;

  for (i = 0 ; i < trie->nb_children ; ++i)
    trie_delete(&trie->children[i]);
  free(trie->children);
}

/*
** Inserts in trie the key-value pair
** Updates if the key already exists
** Returns 0 on success, or a non-zero integer on failure
** Possible Errors :
** - ENOMEM : no more memory available
*/
int		trie_insert(t_trie *trie, const char *key, void *value)
{
  size_t	i;
  t_trie	*child;

  /* If we are at the end of the recursive */
  if (!*key)
    {
      trie->marker = true;
      trie->value = value;
      return (0);
    }

  /* Look for the right child */
  for (i = 0 ; i < trie->nb_children ; ++i)
    if (trie->children[i].letter == *key)
      return (trie_insert(&trie->children[i], key + 1, value));

  /* Allocate a new child if necessary */
  if (trie->nb_children == trie->alloc_children)
    if (trie_realloc_children(trie, trie->alloc_children + TRIE_ALLOC_STEP))
      return (1);

  /* Correctly fill its values */
  child = &trie->children[trie->nb_children];
  ++trie->nb_children;
  trie_init(child);
  child->letter = *key;
  child->parent = trie;
  return (trie_insert(child, key + 1, value));
}

/*
** Check if a key exists in the trie
** Returns true if the key is contained in trie, false otherwise
*/
bool		trie_key_exists(t_trie *trie, const char *key)
{
  size_t	i;

  if (!*key)
    return (trie->marker);;
  for (i = 0 ; i < trie->nb_children ; ++i)
    if (trie->children[i].letter == *key)
      return (trie_key_exists(&trie->children[i], key + 1));
  return (false);
}

/*
** Get value associated with key in the trie
** Returns the value if it is found, NULL otherwise
** errno is set to EINVAL if it is not found, so users may safely set value
** to NULL and check with trie_get_value if key exists
*/
void		*trie_get_value(t_trie *trie, const char *key)
{
  size_t	i;

  if (!*key)
    {
      if (trie->marker)
	return (trie->value);
      errno = EINVAL;
      return (NULL);
    }
  for (i = 0 ; i < trie->nb_children ; ++i)
    if (trie->children[i].letter == *key)
      return (trie_get_value(&trie->children[i], key + 1));
  errno = EINVAL;
  return (NULL);
}

/*
** Deletes a key from a trie
** Returns 0 on success, or a non-zero integer on failure
** Possible Errors :
** - The key is not in the trie
*/
int		trie_delete_key(t_trie *trie, const char *key)
{
  t_trie	*current;
  t_trie	*parent;
  size_t	i;

  if (!*key)
    {
      if (!trie->marker)
	return (1);
      trie->marker = false;
      if (trie->nb_children == 0)
	{
	  /* Delete all unused nodes */
	  current = trie;
	  while (current->parent && current->parent->nb_children == 1 &&
		 !current->parent->marker)
	    current = current->parent;
	  parent = current->parent;
	  trie_delete(current);
	  if (parent)
	    trie_delete_child(parent, current);
	}
      return (0);
    }
  for (i = 0 ; i < trie->nb_children ; ++i)
    if (trie->children[i].letter == *key)
      return (trie_delete_key(&trie->children[i], key + 1));
  return (1);
}

/*
** Sorts contents in the trie
** This does NOT make the trie faster
** It may only be useful if you want to order before traversal
** It uses qsort from stdlib
*/
void		trie_sort(t_trie *trie)
{
  size_t	i;

  if (trie->nb_children > 1)
    {
      qsort(trie->children, trie->nb_children, sizeof(t_trie),
	    &trie_qsort_predicate);
      for (i = 0 ; i < trie->nb_children ; ++i)
	{
	  trie->children[i].parent = trie;
	  trie_sort(&trie->children[i]);
	}
    }
  else if (trie->nb_children == 1)
    trie_sort(&trie->children[0]);
}

/*
** Traverses the trie using pre order traversal
** Children are called from the first to the last (use trie_sort if you need
** them sorted by ascii value)
** fct is a function that will be called with the following parameters :
** - The key
** - The value
** - The data (third parameter passed to trie_traversal_preorder)
** fct is expected to return 0 on success, or a non-zero integer on failure
** When fct fails, the traversal immediately stops, and trie_traversal_preorder
** returns a non-zero integer.
** Returns 0 on success, a non-zero integer on failure
** Possible Errors :
** - fct returned non-zero
** - ENOMEM : no more memory available
*/
int	trie_traversal_preorder(
    t_trie *trie, int (*fct)(const char *, void *, void *), void *data)
{
  size_t	i;
  char		*key;

  key = malloc(TRAVERSAL_KEY_STEP);
  memset(key, 0, TRAVERSAL_KEY_STEP);
  if (!key)
    return (1);
  for (i = 0 ; i < trie->nb_children ; ++i)
    if (trie_traversal_preorder_recur(&trie->children[i], fct, data, &key, 0,
				      TRAVERSAL_KEY_STEP))
      {
	free(key);
	return (1);
      }
  free(key);
  return (0);
}

/*
** Same as trie_traversal_preorder but using postorder instead
*/
int	trie_traversal_postorder(
    t_trie *trie, int (*fct)(const char *, void *, void *), void *data)
{
  size_t	i;
  char		*key;

  key = malloc(TRAVERSAL_KEY_STEP);
  memset(key, 0, TRAVERSAL_KEY_STEP);
  if (!key)
    return (1);
  for (i = 0 ; i < trie->nb_children ; ++i)
    if (trie_traversal_postorder_recur(&trie->children[i], fct, data, &key, 0,
				       TRAVERSAL_KEY_STEP))
      {
	free(key);
	return (1);
      }
  free(key);
  return (0);
}
