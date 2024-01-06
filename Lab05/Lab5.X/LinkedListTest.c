/* Kevin Truong
 * Collaborated with: N/A
 * Test Harness for LinkedList.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BOARD.h"

#include "LinkedList.h"

#define String1 "Dog"
#define String2 "Rat"
#define String3 "Dragon"

int main(void){
    BOARD_Init();
    
    printf("\nWelcome to ktruon13's LinkedListTest.c, compiled on %s %s.\n\n", __DATE__, __TIME__);
    
    // test for LinkedListNew()
    char *data = "Dog";
    ListItem *List1 = LinkedListNew(data);
    printf("\nTesting LinkedListNew: \n\tExpected Result 'Dog': %s\n", List1->data);
    
    if (!strcmp(List1->data, String1)) {
        printf("\tLinkedListNew: Passed!\n");
    } else {
        printf("\tFailed!\n");
    }
    
    // test for LinkListCreateAfter()
    char *data2 = "Dragon";
    ListItem *List2 = LinkedListCreateAfter(List1, data2);
    printf("\nTesting LinkedListCreateAfter: \n\tExpected Result 'Dog Dragon': %s %s\n", 
            List2->previousItem->data, List2->data);
    
    char *data3 = "Rat";
    ListItem *List3 = LinkedListCreateAfter(List1, data3);
    printf("\tExpected Result 'Dog Rat Dragon': %s %s %s\n", 
            List3->previousItem->data, List3->data, List3->nextItem->data);
    
    if (!strcmp(List3->previousItem->data, String1) && !strcmp(List3->data, String2)
            && !strcmp(List3->nextItem->data, String3)) {
        printf("\tLinkedListCreateAfter: Passed!\n");
    } else {
        printf("\tFailed!\n");
    }
    
    // test for LinkedListRemove()
    char *data4 = LinkedListRemove(List2);
    printf("\nTesting LinkedListRemove: \n\tExpected Result 'Dog Rat' (removed = Dragon): "
            "%s %s (removed = %s)\n", List3->previousItem->data, List1->nextItem->data, data4);
    List2 = LinkedListCreateAfter(List3, data2);
    
    char *data5 = LinkedListRemove(List3);
    printf("\tExpected Result 'Dog Dragon' (removed = Rat): "
            "%s %s (removed = %s)\n", List2->previousItem->data, List1->nextItem->data, data5);
    List3 = LinkedListCreateAfter(List1, data3);
    
    char *data6 = LinkedListRemove(List1);
    printf("\tExpected Result 'Rat Dragon' (removed = Dog): "
            "%s %s (removed = %s)\n", List2->previousItem->data, List3->nextItem->data, data6);
    
    if (data4 && data5 && data6) {
        printf("\tLinkedListRemove: Passed!\n");
    } else {
        printf("\tFailed!\n");
    }
    
    // test for LinkedListGetFirst()
    List1 = LinkedListGetFirst(List2);
    printf("\nTesting LinkedListGetFirst: \n\tExpected Result 'Rat': %s\n", List1->data);
    List1 = LinkedListGetFirst(List3);
    printf("\tExpected Result 'Rat': %s\n", List1->data);
    
    if (!strcmp(List1->data, String2)) {
        printf("\tLinkedListGetFirst: Passed!\n");
    } else {
        printf("\tFailed!\n");
    }
    
    // test for LinkedListGetLast()
    List1 = LinkedListGetLast(List2);
    printf("\nTesting LinkedListGetLast: \n\tExpected Result 'Dragon': %s\n", List1->data);
    List1 = LinkedListGetLast(List3);
    printf("\tExpected Result 'Dragon': %s\n", List1->data);
    
    if (!strcmp(List1->data, String3)) {
        printf("\tLinkedListGetLast: Passed!\n");
    } else {
        printf("\tFailed!\n");
    }
    
    // test for LinkedListSize()
    int size = LinkedListSize(List2);
    printf("\nTesting LinkedListSize: \n\tExpected Result 2: %d\n", size);
    
    if (size == 2) {
        printf("\tLinkedListSize: Passed!\n");
    } else {
        printf("\tFailed!\n");
    }
    
    // test for LinkedListSwapData()
    int swap = LinkedListSwapData(List2, List3);
    printf("\nTesting LinkedListSwapData: \n\t");
    if (swap) {
        printf("Expected Result 'Dragon Rat': %s %s\n",
                List3->data, List2->data);
    } else {
        printf("failed!\n");
    }
    swap = LinkedListSwapData(List2, List3);
    if (swap) {
        printf("\tExpected Result 'Rat Dragon': %s %s\n",
                List3->data, List2->data);
    }
    LinkedListSwapData(List2, List3);
    
    if (!strcmp(List2->data, String2) && !strcmp(List3->data, String3)) {
        printf("\tLinkedListGetSwapData: Passed!\n");
    } else {
        printf("\tFailed!\n");
    }
    
    // test for LinkedListPrint
    printf("\nTesting LinkedListPrint: \n\tExpected Result '[Dragon, Rat]': ");
    LinkedListPrint(List3);
    List2->data = NULL;
    printf("\tExpected Result '[Dragon, (null)]': ");
    LinkedListPrint(List2);
    
    ListItem *List4 = NULL;
    ListItem *List5 = NULL;
    
    printf("\nTesting NULL inputs: \n\t");
    if (LinkedListRemove(List4) == NULL && LinkedListSize(List4) == 0 
            && LinkedListGetFirst(List4) == NULL && LinkedListGetLast(List4) == NULL 
            && LinkedListSwapData(List4, List5) == STANDARD_ERROR) {
        printf("All tests passed: (5/5) \n");
    } else {
        printf("failed!\n");
    }
    
    
    BOARD_End();
    while(1);
    return 0;
}