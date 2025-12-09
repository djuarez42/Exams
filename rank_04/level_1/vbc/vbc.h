/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vbc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:57:02 by marvin            #+#    #+#             */
/*   Updated: 2025/11/14 13:57:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VBC_H
#define VBC_H

# include <stdio.h>
# include <malloc.h>
# include <ctype.h>
# include <stdlib.h>

typedef struct node 
{
    enum 
    {
        ADD,
        MULTI,
        VAL
    }   type;
    int		val;
    struct	node *l;
    struct	node *r;
}   node;

node    *new_node(node n);
void    destroy_tree(node *n);
void    unexpected(char c);
int	    accept(char **s, char c);
int	    expect(char **s, char c);
node    *parse_expr(char *s);
int	eval_tree(node *tree);
//To implement
node	*parse_factor(char **s);
node	*parse_term(char **s);
node	*parse_sum(char **s);

#endif

