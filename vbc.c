#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum precedence {
    para=-1, add=1, multi=2, expo=3
} pre;

typedef struct op
{
    char operation;
    struct op *next;
    pre order;
}t_op;

typedef struct num
{
    int n;
    char operation;
    struct num *next;
    struct num *prev;
    pre order;
}t_num;

typedef struct stack
{
    int n;
    char operation;
    struct stack *next;

}t_s;

t_s *ft_new_ts()
{
    t_s *new = malloc(sizeof(t_s));
    if (!new)
        return (NULL);
    new->n = -1;
    new->next = NULL;
    return (new);
}

t_op    *ft_new_stack()
{
    t_op *stack;
    stack = malloc(sizeof(t_op));
    if (!stack)
        return (NULL);
    stack->next = NULL;
    return (stack);
}

t_num *ft_new_que()
{
    t_num *que;
    que = malloc(sizeof(t_num));
    if (!que)
        return (NULL);
    que->n = -1;
    que->next = NULL;
    que->prev = NULL;
    return (que);
}

void    pop_num(t_num **h_que, t_num *n_new)
{
    n_new->next = (*h_que);
    (*h_que)->prev = n_new;
    *h_que = n_new;
}
void    pop_stack(t_op **h_stack, t_op *new)
{
    new->next = (*h_stack);
    (*h_stack) = new;
}

pre ft_order(char c)
{
    if (c == '+' || c == '-')
        return (add);
    else if (c == '*' || c == '/' || c == '%')
        return (multi);
    return (para);
}

void    insert(t_op **h_stack, t_num **h_que, int check, char c, int number)
{
    t_op *new = NULL;
    t_op *tmp = NULL;
    t_num *n_new = NULL;
    t_num *temp = NULL;
    pre my_order;
    if (check == 1)
    {
        if (!*h_stack)
        {
            new = ft_new_stack();
            if (!new)
                return ;
            *h_stack = new;
            (*h_stack)->operation = c;
            (*h_stack)->order = ft_order(c);
        }
        else
        {
            my_order = ft_order(c);
            if (c != ')' && c != '(' && ((*h_stack)->order > my_order || (*h_stack)->order == my_order))
            {
                n_new = ft_new_que();
                if (!n_new)
                    return ;
                n_new->operation = (*h_stack)->operation;
                pop_num(h_que, n_new);
                (*h_stack) = (*h_stack)->next;
                new = ft_new_stack();
                if (!new)
                    return ;
                new->operation = c;
                new->order = ft_order(c);
                pop_stack(h_stack, new);
            }
            else if (c == ')')
            {
                while ((*h_stack) && (*h_stack)->operation != '(')
                {
                    n_new = ft_new_que();
                    if (!n_new)
                        return ;
                    n_new->operation = (*h_stack)->operation;
                    pop_num(h_que, n_new);
                    (*h_stack) = (*h_stack)->next;
                }
                (*h_stack) = (*h_stack)->next;
            }
            else
            {
                new = ft_new_stack();
                if (!new)
                    return ;
                new->operation = c;
                new->order = ft_order(c);
                pop_stack(h_stack, new);
            }
        }

    }
    else if (check == 2)
    {
        if (!*h_que)
        {
            n_new = ft_new_que();
            if (!n_new)
                return ;
            (*h_que) = n_new;
            (*h_que)->n = number;
        }
        else
        {
            n_new = ft_new_que();
            if (!n_new)
                return ;
            n_new->n = number;
            pop_num(h_que, n_new);
        }
    }
}

int ft_strlen(char *s)
{
    int i = 0;
    while (s[i])
        i++;
    return (i);
}

int count_para(char *s)
{
    int i = 0;
    int left = 0;
    int right = 0;
    while (s[i])
    {
        if (s[i] == '(')
            left++;
        else if (s[i] == ')')
            right++;
        i++;
    }
    if (left != right)
        return (1);
    return (0);
}

int is_op(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/')
        return (1);
    return (0);
}

void ft_parsing(char *s)
{
    int i = 0;
    char c;
    int lent = ft_strlen(s);
    if (!s)
        return ;
    if (!isdigit(s[lent-1]) && s[lent-1] != '(' && s[lent-1] != ')')
    {
        printf("Unexpected end of input\n");
        exit (1);
    }
    while (s[i])
    {
        if (s[i] == '(' || s[i] == ')')
            c = s[i];
        if (is_op(s[i]))
        {
            if (is_op(s[i+1]))
            {
                printf("multiple operators\n");
                exit (1);
            }
            if (s[i+1] == ')' || s[i+1] == '(')
                return (printf("Unexpected token '%c'\n", s[i+1]), exit(1));
        }
        i++;
    }
    if (count_para(s))
    {
        printf("Unexpected token '%c'\n", c);
        exit (1);
    }
}

int ft_atoi(char *s)
{
    int i = 0;
    int num = 0;
    while (s[i] && isdigit(s[i]))
    {
        num = num * 10 + (s[i] - 48);
        i++; 
    }
    return (num);
}

void    pop_everything(t_op **h_stack, t_num **h_que)
{
    t_num *n_new = NULL;
    while ((*h_stack))
    {
        n_new = ft_new_que();
        if (!n_new)
            return ;
        n_new->operation = (*h_stack)->operation;
        pop_num(h_que, n_new);
        (*h_stack) = (*h_stack)->next;
    }
}

void    pop_to_stack(t_s **head, int number)
{
    t_s *new = NULL;
    if (!*head)
    {
        new = ft_new_ts();
        if (!new)
            return ;
        *head = new;
        (*head)->n = number;
    }
    else
    {
        new = ft_new_ts();
        if (!new)
            return ;
        new->n = number;
        new->next = *head;
        *head = new;
    }
}

void    ft_delete(t_s **head)
{
    *head = (*head)->next->next;
}

int ft_lent(t_num *head)
{
    int count = 0;
    while (head)
    {
        count++;
        head = head->next;
    }
    return (count);
}

void    post_fix(t_num **h_que)
{
    t_num *n_new = NULL;
    t_s *head = NULL;
    int result = 0;
    int first = 0;
    int second = 0;
    int lent = 0;
    t_num *tmp = *h_que;
    t_num *last = NULL;
    while (tmp->next)
        tmp = tmp->next;
    last = tmp;
    lent = ft_lent(*h_que);
    if (lent == 1)
        result = (*h_que)->n;
    else
    {
        while (lent > 0)
        {
            if (last->n != -1)
                pop_to_stack(&head, last->n);
            else
            {
                first = (head->next->n);
                second = head->n;
                if (last->operation == '*')
                    result = first * second;
                else if (last->operation == '+')
                    result = first + second;
                else if (last->operation == '-')
                    result = first - second;
                else if (last->operation == '/')
                    result = first / second;
                head = head->next->next;
                pop_to_stack(&head, result);
            }
            last = last->prev;
            lent--;
        }
    }
    printf("result: %d\n", result);
}

void    print(t_num *h_que)
{
    while ((h_que))
    {
        printf("this is num %d and op %c\n", h_que->n, h_que->operation);
        h_que = h_que->next;
    }
}

void    print_stack(t_op *h_stack)
{
    while (h_stack)
    {
        printf("this is stack op %c\n", h_stack->operation);
        h_stack = h_stack->next;
    }
}

void    ft_vbc(char *s)
{
    int i = 0;
    int num = 0;
    char c;
    t_op *h_stack = NULL;
    t_num *h_que = NULL;
    if (!s)
        return ;
    ft_parsing(s);
    while (s[i])
    {
        if (isdigit(s[i]))
        {
            num = ft_atoi(&s[i]);
            insert(&h_stack, &h_que, 2, 0, num);
        }
        else
        {
            c = s[i];
            insert(&h_stack, &h_que, 1, c, 0);
        }
        i++;
    }
    print_stack(h_stack);
    print(h_que);
    pop_everything(&h_stack, &h_que);
    post_fix(&h_que);
}

int main(int ac, char *argv[])
{
    if (ac != 2)
    {
        printf("not enough arguments\n");
        return (1);
    }
    ft_vbc(argv[1]);
}