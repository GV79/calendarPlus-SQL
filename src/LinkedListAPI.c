/***************************************************************************************
CIS2750 F2017
Assignment 0
Gia Vinh Lam
0925571

Ths function contains the implementation of an  abstract linked list with an iterator.
Details regarding functions can be found in the header file LinkedListAPI.h

CIS2520 (Fangju Wang) - Looked at my linked list code to refresh my knowledge.
Permission to use this code was granted by my current cis2750 professor Denis Nitikenko

Compiles with -std=c11 -Wall and -pedantic tags.
****************************************************************************************/

//need to test functions when list isnt initialized or no node in there

#include "LinkedListAPI.h"

/*
char* printFunc(void *toBePrinted){
	return (char*)toBePrinted;
}

//Comparing strings is done by strcmp
int compareFunc(const void *first, const void *second){
	return strcmp((char*)first, (char*)second);
}

//Freeing a string is also straightforward
void deleteFunc(void *toBeDeleted){
	free(toBeDeleted);
}


int main()
{
    List list = initializeList(&printFunc, &deleteFunc, &compareFunc);

if ((char*)getFromFront(list) != NULL)
{
printf("%s\n", (char*)getFromFront(list));
}
    insertFront(&list, "upple");
    insertBack(&list, "wrap");
    insertSorted(&list, "test");
    insertSorted(&list, "kangaroo");
    insertSorted(&list, "oompa");
    insertSorted(&list, "zebra");
    insertSorted(&list, "kangaroo");
insertFront(&list, "aaah");
insertBack(&list, "woah");
insertSorted(&list, "ab");
insertSorted(&list, "zzz");
printf("%s %s\n", (char*)getFromFront(list), (char*)getFromBack(list));
printf("%s\n", toString(list));
deleteDataFromList(&list, "zzz");
    //printf("%s\n", toString(list));
    //insertBack(&list, "test3"
    //insertFront(&list, "test");
    //insertBack(&list, "test2");

    if (toString(list) != NULL)
    {
    printf("%s\n", toString(list));
    }
//    ListIterator iter = createIterator(list);
//    printf("next element %s\n", (char*)nextElement(&iter));
//    printf("next element %s\n", (char*)nextElement(&iter));
//    printf("next element %s\n", (char*)nextElement(&iter));
    clearList(&list);
    return 0;
}
*/

List initializeList(char* (*printFunction)(void* toBePrinted),void (*deleteFunction)(void* toBeDeleted),int (*compareFunction)(const void* first,const void* second))
{
    List list;
    list.head = NULL;
    list.tail = NULL;
    list.deleteData = deleteFunction;
    list.compare = compareFunction;
    list.printData = printFunction;
    return list;
}

Node * initializeNode(void * data)
{
    Node * node = malloc(sizeof(Node));
    if (node == NULL)
    {
        printf("Memory allocation has failed in function initializeNode. Program exiting.\n");
        exit(0);
    }

    if (node == NULL || data == NULL)
    {
        printf("Data is null.\n");
        return NULL;
    }
    else
    {
        node->data = data;
        node->previous = NULL;
        node->next = NULL;
        return node;
    }
}


void insertFront(List * list, void * toBeAdded)
{
    Node * newNode = initializeNode(toBeAdded);
    if (list->head == NULL)
    {
        list->head = newNode;
        list->tail = newNode;
        return;
    }
    else
    {
        list->head->previous = newNode;
        newNode->next = list->head;
        newNode->previous = NULL;
        list->head = newNode;
    }
    return;
}



void insertBack(List * list, void * toBeAdded)
{
    Node * newNode = initializeNode(toBeAdded);
    if (list->head == NULL)
    {
        list->head = newNode;
        list->tail = newNode;
        return;
    }
    else
    {
        newNode->previous = list->tail;
        list->tail->next = newNode;
        list->tail = newNode;
        list->tail->next = NULL;
    }
    return;
}

void clearList(List* list)
{
    if (list->head != NULL)
    {
        Node * temp = list->head;
        while (temp->next != NULL)
        {
            list->deleteData(temp);
            temp = temp->next;
        }
        list->deleteData(list->tail);
    }
    return;
}

void insertSorted(List* list, void* toBeAdded)
{
    Node * newNode = initializeNode(toBeAdded);
    if (newNode == NULL)
    {
        printf("Data is null. Unable to insert element.\n");
        return;
    }

    if (list->head == NULL)
    {
        list->head = newNode;
        list->tail = newNode;
        return;
    }
    while (list->head != NULL)
    {
        if (list->compare(list->head->data, toBeAdded) <= 0) //indicates str1 is less than or equal to str2. In the ASCII table, B and a is less than A
        {
            if (list->head == list->tail) // If there is only one node currently... ex. New Node: B, Old Node: A
            {
                list->head->next = newNode;
                newNode->previous = list->head;
                newNode->next = NULL;
                list->tail = newNode;

                while (list->head->previous != NULL)
                {
                    list->head = list->head->previous;
                }
                return;
            }

            if (list->compare(list->head->next->data, toBeAdded) >= 0)
            {
                newNode->next = list->head->next;
                newNode->previous = list->head;
                list->head->next->previous = newNode;
                list->head->next = newNode;
                while (list->head->previous != NULL)
                {
                    list->head = list->head->previous;
                }
                return;
            }
        }
        else
        {
            list->head->previous = newNode;
            newNode->previous = NULL;
            newNode->next = list->head;
            list->head = newNode;
            return;
        }
        list->head = list->head->next;
    }
    return;
}

void* deleteDataFromList(List* list, void* toBeDeleted)
{
    void * dataDeleted = toBeDeleted;
    Node * temp = list->head;

    if (list == NULL)
    {
        printf("List does not exist.\n");
        return NULL;
    }
    else
    {
        while (list->head != NULL)
        {
            if (list->compare(list->head->data, toBeDeleted) == 0)
            {
                if (list->head->previous == NULL && list->head->next == NULL)
                {
                    list->deleteData(temp);
                    list->head = NULL;
                    list->tail = NULL;
                    return NULL;
                }

                if (list->head->previous == NULL)
                {
                    list->head->next->previous = NULL;
                    temp = list->head;
                    list->head = list->head->next;
                    dataDeleted = temp->data;
                    free(temp);
                    return dataDeleted;
                }
                else if (list->head->next == NULL)
                {
                    list->head->previous->next = NULL;
                    list->tail = list->head->previous;
                    temp = list->head;
                    while (list->head->previous != NULL)
                    {
                        list->head = list->head->previous;
                    }
                    dataDeleted = temp->data;
                    free(temp);
                    return dataDeleted;
                }
                else
                {
                    temp = list->head;
                    list->head->previous->next = list->head->next;
                    list->head->next->previous = list->head->previous;
                    while (list->head->previous != NULL)
                    {
                        list->head = list->head->previous;
                    }
                    dataDeleted = temp->data;
                    free(temp);
                    return dataDeleted;
                }
            }
            list->head = list->head->next;
        }
        list->head = temp;
        printf("Unable to find/delete node \"%s\".\n", (char*)toBeDeleted);
        return NULL;
    }
}

void * getFromFront(List list)
{
    if (list.head == NULL)
    {
        return NULL;
    }
    return list.head->data;
}


void * getFromBack(List list)
{
    if (list.tail == NULL)
    {
        return NULL;
    }
    return list.tail->data;
}

char * toString(List list)
{
    List temp = list;
    char * listData;
    int amount = 0;
    if (list.head != NULL)
    {
        while (temp.head != NULL)
        {
            amount = amount + strlen(list.printData(temp.head->data))+3;
            temp.head = temp.head->next;
        }

        listData = malloc(sizeof(char*) * amount);
        while (list.head != NULL)
        {
            listData = strcat(listData, list.printData(list.head->data));
            listData = strcat(listData, " ");
            list.head = list.head->next;
        }
        return listData;
    }
    printf("List is empty and thus printing this may cause a seg fault (depending on how it is printed).\n");
    return NULL;
}

ListIterator createIterator(List list)
{
    ListIterator iter;

    if (list.head != NULL)
    {
        iter.current = list.head;
    }
    return iter;
}

void * nextElement(ListIterator * iter)
{
    void * storage;
    if (iter->current != NULL)
    {
        storage = iter->current->data;
        iter->current = iter->current->next;
        return storage;
    }
    return NULL;
}
