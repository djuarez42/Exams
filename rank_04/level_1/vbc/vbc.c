/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vbc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 23:55:49 by marvin            #+#    #+#             */
/*   Updated: 2025/11/01 23:55:49 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vbc.h"

node    *new_node(node n)
{
	node *ret = calloc(1, sizeof(node));
	if (!ret)
		return (NULL);
	*ret = n;
	return (ret);
}

void    destroy_tree(node *n)
{
	if (!n)
		return ;
	if (n->type != VAL)
	{
		destroy_tree(n->l);
		destroy_tree(n->r);
	}
	free(n);
}

void    unexpected(char c)
{
	if (c)
		printf("Unexpected token '%c'\n", c);
	else
		printf("Unexpected end of input\n");
}

int accept(char **s, char c)
{
	if (**s == c)
	{
		(*s)++;
		return (1);
	}
	return (0);
}

int expect(char **s, char c)
{
	if (accept(s, c))
		return (1);
	unexpected(**s);
	return (0);
}

node	*parse_factor(char **s)
{
	int     val;
	node	*ret;
	node	*new_val_node;

	if (isdigit(**s))
	{
		val = **s - '0';
		(*s)++;
		new_val_node = new_node((node){VAL, val, NULL, NULL});
		if (!new_val_node)
			return (NULL);
		return (new_val_node);
	}
	else if (**s == '(')
	{
		(*s)++;
		ret = parse_sum(s);
		if (!ret || !expect(s, ')'))
		{
			destroy_tree(ret);
			return (NULL);
		}
		return (ret);
	}
	unexpected(**s);
	return (NULL);
}

node	*parse_term(char **s)
{
	node	*left;
	node	*right;

	left = parse_factor(s);
	if (!left)
		return (NULL);
	while (**s == '*')
	{
		(*s)++;
		right = parse_factor(s);
		if (!right)
		{
			destroy_tree(left);
			return (NULL);
		}
		left = new_node((node){MULTI, 0, left, right});
		if (!left)
		{
			destroy_tree(right);
			return (NULL);
		}
	}
	return (left);
}

node    *parse_sum(char **s)
{
	node    *left;
	node    *right;

	left = parse_term(s);
	if (!left)
		 return (NULL);
	while (**s == '+')
	{
		(*s)++;
		right = parse_term(s);
		if (!right)
		{
			destroy_tree(left);
				return (NULL);
		}
		left = new_node((node){ADD, 0, left, right});
		if (!left)
		{
				destroy_tree(right);
				return (NULL);
		}
	}
	return (left);
}

node    *parse_expr(char *s)
{
	node	*ret;

	if (!s)
	return (NULL);
	ret = parse_sum(&s);
	if (!ret)
	return (NULL);
	if (*s) 
	{
		unexpected(*s); // <- importante para el ultimo ejemplo del subject
		destroy_tree(ret);
		return (NULL);
	}
	return (ret);
}

int eval_tree(node *tree)
{
	switch (tree->type)
	{
		case ADD:
			return (eval_tree(tree->l) + eval_tree(tree->r));
		case MULTI:
			return (eval_tree(tree->l) * eval_tree(tree->r));
		case VAL:
			return (tree->val);
	}
	return (0);
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	node *tree = parse_expr(argv[1]);
	if (!tree)
		return (1);
	printf("%d\n", eval_tree(tree));
	destroy_tree(tree);
	return (0);
}
