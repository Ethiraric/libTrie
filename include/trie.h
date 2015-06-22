/*
** trie.h for  in /home/sabour_f/rendu/libTrie/include
**
** Made by Florian SABOURIN
** Login   <sabour_f@epitech.net>
**
** Started on  Wed Jun 17 19:41:04 2015 Florian SABOURIN
** Last update Wed Jun 17 19:41:04 2015 Florian SABOURIN
*/

#ifndef TRIE_H_
# define TRIE_H_

# include <stdbool.h>
# include <sys/types.h>

typedef struct s_trie t_trie;

/*
** Main structure for a trie
*/
struct	s_trie
{
  t_trie	*children;	// Array for t_trie for child nodes
  t_trie	*parent;	// Parent node
  size_t	nb_children;	// Number of children
  size_t	alloc_children;	// Number of allocated children
  char		letter;		// Letter of the key of the current node
  void		*value;		// Value of the current node
  bool		marker;		// Marks the end of a word
};

void	trie_init(t_trie *trie);
void	trie_delete(t_trie *trie);
int	trie_insert(t_trie *trie, const char *key, void *value);
bool	trie_key_exists(t_trie *trie, const char *key);
void	*trie_get_value(t_trie *trie, const char *key);
int	trie_delete_key(t_trie *trie, const char *key);
void	trie_sort(t_trie *trie);
int	trie_traversal_preorder(
    t_trie *trie, int (*fct)(const char *, void *, void *), void *data);
int	trie_traversal_postorder(
    t_trie *trie, int (*fct)(const char *, void *, void *), void *data);

#endif /* !TRIE_H_ */
