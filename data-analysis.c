#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>
typedef struct node
{
    double value;
    int timestamp;
    struct node *next;
    struct node *prev;
    int rmv;
} node;
node *ultim;
node *insertNode(int timestamp, double value, node **head)
{
    node *temp = (node *)malloc(sizeof(node));
    temp->timestamp = timestamp;
    temp->value = value;
    temp->next = NULL;
    temp->rmv = 0;
    if (*head == NULL)
    {
        temp->prev = NULL;
        *head = temp;
    }
    else
    {
        temp->prev = ultim;
        ultim->next = temp;
    }
    ultim = temp;
    return temp;
}
void readList(int n, node **head)
{
    int timestamp;
    double value;
    int i;
    for (i = 0; i < n; i++)
    {
        scanf("%d%lf", &timestamp, &value);
        insertNode(timestamp, value, head);
    }
}
void printList(node *head)
{
    node *temp = head;
    while (temp != NULL)
    {
        printf("%d %.2f\n", temp->timestamp, temp->value);
        temp = temp->next;
    }
}

void removeNode(node **head, node *rm_node)
{
    if (rm_node->prev == NULL)
    {
        rm_node->next->prev = NULL;
        *head = rm_node->next;
    }
    else if (rm_node->next == NULL)
        rm_node->prev->next = NULL;
    else
    {
        rm_node->prev->next = rm_node->next;
        rm_node->next->prev = rm_node->prev;
    }
}
void freeList(node *head)
{
    node *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}
void dev_cmp(node *head, node *nod)
{
    node *temp_n = nod, *temp_p = nod;
    double sum = nod->value;
    int i;
    for (i = 0; i < 5 / 2; i++)
    {
        temp_n = temp_n->next;
        sum += temp_n->value;
        temp_p = temp_p->prev;
        sum += temp_p->value;
    }
    sum = sum / 5;
    double dev = 0;
    for (i = 0; i < 5; i++)
    {
        dev += ((temp_p->value - sum) * (temp_p->value - sum));
        temp_p = temp_p->next;
    }
    dev = (double)sqrtf(dev / 5);
    if (nod->value > (sum + dev) || nod->value < (sum - dev))
    {
        nod->rmv = 1;
    }
}
void rmv_e1(node *head)
{
    node *nod = head;
    nod = nod->next->next;
    while (nod->next->next != NULL)
    {
        dev_cmp(head, nod);
        nod = nod->next;
    }
}
int remove_marked(node **head)
{
    node *temp = *head, *aux;
    int rmvc = 0;
    while (temp != NULL)
    {
        aux = temp;
        temp = temp->next;
        if (aux->rmv == 1)
        {
            removeNode(head, aux);
            free(aux);
            rmvc++;
        }
    }
    return rmvc;
}
double find_median(node *nod)
{
    double max1, max2, max3;
    node *temp = nod->prev->prev;
    max1 = max2 = max3 = -DBL_MAX;
    int i;
    for (i = 0; i < 5; i++)
    {
        if (temp->value > max1)
            max1 = temp->value;
        temp = temp->next;
    }
    temp = nod->prev->prev;
    for (i = 0; i < 5; i++)
    {
        if (temp->value > max2 && temp->value < max1)
            max2 = temp->value;
        temp = temp->next;
    }
    temp = nod->prev->prev;
    for (i = 0; i < 5; i++)
    {
        if (temp->value > max3 && temp->value < max2)
            max3 = temp->value;

        temp = temp->next;
    }
    return max3;
}
void create_median_list(node *prev_head, node **head_med)
{
    node *temp = prev_head->next->next;
    while (temp->next->next != NULL)
    {
        insertNode(temp->timestamp, find_median(temp), head_med);
        temp = temp->next;
    }
}
//daca as fi facut separat functia de average la taskul 2.1 puteam sa o folosesc direct aici
double calc_avg(node *nod)
{
    double avg = 0;
    nod = nod->prev->prev;
    int i;
    for (i = 0; i < 5; i++)
    {
        avg += nod->value;
        nod = nod->next;
    }
    return avg / 5;
}
void create_avg_list(node *prev_head, node **head_avg)
{
    node *temp = prev_head->next->next;
    while (temp->next->next != NULL)
    {
        insertNode(temp->timestamp, calc_avg(temp), head_avg);
        temp = temp->next;
    }
}
void uniform_list(node *head)
{
    node *temp = head->next;
    while (temp != NULL)
    {
        int diff = temp->timestamp - temp->prev->timestamp;
        if (diff >= 100 && diff <= 1000)
        {
            temp->timestamp = (temp->timestamp + temp->prev->timestamp) / 2;
            temp->value = (temp->value + temp->prev->value) / 2;
        }
        temp = temp->next;
    }
}
void insert_at_timestamp(int key_timestamp, int timestamp, double value, node **head)
{
    node *temp = *head;
    while (temp != NULL)
    {
        if (temp->timestamp == key_timestamp)
        {
            node *aux = (node *)malloc(sizeof(node));
            aux->timestamp = timestamp;
            aux->value = value;
            aux->rmv = 0;
            aux->next = temp;
            aux->prev = temp->prev;
            temp->prev->next = aux;
            temp->prev = aux;
        }
        temp = temp->next;
    }
}
int data_completion(node **head)
{
    node *temp = *head, *left_temp = NULL, *right_temp = NULL;
    int i, k = 0;
    temp = (*head)->next;
    while (temp != NULL)
    {
        int diff = temp->timestamp - temp->prev->timestamp;
        int add = 200;
        if (diff > 1000)
        {
            int stamp = temp->prev->timestamp;
            node *aux = temp->prev->prev->prev;
            while (add < diff)
            {
                double value;
                left_temp = aux;
                right_temp = temp->next->next;
                
                double C = (double)((stamp * 1.0 + add * 1.0) - left_temp->next->next->timestamp * 1.0) / 
                (double)(right_temp->prev->prev->timestamp * 1.0 - left_temp->next->next->timestamp * 1.0);
                double left_sum = 0, right_sum = 0;
                double sum = 0;
                int j;
                for (j = 0; j < 3; j++)
                    sum += (((j * 1.0 / 2) * (j * 1.0 / 2)) * 0.9 + 0.1);
                for (i = 0; i < 3; i++)
                {
                    double w = (((i * 1.0 / 2.0) * (i * 1.0 / 2.0)) * 0.9 + 0.1) / sum;
                    left_sum += left_temp->value * w;
                    left_temp = left_temp->next;
                    right_sum += right_temp->value * w;
                    right_temp = right_temp->prev;
                }
                value = (1 - C) * left_sum + C * right_sum;

                insert_at_timestamp(temp->timestamp, stamp + add, value, head);
                k++;
                add += 200;
            }
        }
        temp = temp->next;
    }
    return k;
}
void calc_interv(node *head, int length, int n)
{
    node *temp = head;
    int l_bound, u_bound;
    double min = head->value, max = head->value;
    while (temp != NULL)
    {
        if (temp->value < min)
            min = temp->value;
        temp = temp->next;
    }
    l_bound = floor(min);
    u_bound = l_bound + length;
    temp = head;
    while (temp != NULL)
    {
        if (temp->value > max)
            max = temp->value;
        temp = temp->next;
    }
    temp = head;

    while (l_bound < max)
    {
        int c = 0;
        temp = head;
        while (temp != NULL)
        {
            if (temp->value >= l_bound && temp->value <= u_bound)
                c++;
            temp = temp->next;
        }
        if (c > 0)
            printf("[%d, %d] %d\n", l_bound, u_bound, c);
        l_bound = u_bound;
        u_bound += length;
    }
}
int main(int argc, char **argv)
{
    node *head = NULL, *head_aux;
    node *head_med = NULL, *head_avg = NULL;

    int n;
    scanf("%d", &n);
    readList(n, &head);
    head_aux = head;
    int i;
    for (i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "--e1"))
        {
            rmv_e1(head);
            n -= remove_marked(&head);
        }
        if (!strcmp(argv[i], "--e2"))
        {
            create_median_list(head, &head_med);
            n -= 4;
            head = head_med;
        }
        if (!strcmp(argv[i], "--e3"))
        {
            create_avg_list(head, &head_avg);
            n -= 4;
            head = head_avg;
        }
        if (!strcmp(argv[i], "--u"))
        {
            uniform_list(head);
        }
        if (!strcmp(argv[i], "--c"))
        {
            n += data_completion(&head);
        }
        if (strstr(argv[i], "--st"))
        {
            char *s = strcpy(argv[i], argv[i] + 4);
            calc_interv(head, atoi(s), n);
        }
        if (argc - 1 == i && strstr(argv[i], "--"))
        {
            printf("%d\n", n);
            printList(head);
        }
    }
    freeList(head_aux);
    freeList(head_med);
    freeList(head_avg);
    return 0;
}