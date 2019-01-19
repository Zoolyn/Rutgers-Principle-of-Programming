#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"

int main()
{
	Instruction *head;
 	head = ReadInstructionList(stdin);
 	if (!head) {
 		ERROR("No instructions\n");
 		exit(EXIT_FAILURE);
 	}
 	//check the second operation, should be loadi for optimization
 	Instruction *second;
 	//check the third operation after the ptr, this should be an operator like add sub or mul
 	Instruction *third;
 	//this is going to check the list of operations to see if we have to optimize it
	Instruction *temp;
	//this is what points to what is going through the operations
	Instruction *ptr = head;
	//this is a counter that keeps track of how many pairs of LOADIs there are with an operation at the end
	int counter = 0;

	temp = head;
	while(temp != NULL){
		if(temp->opcode == LOADI && (temp->opcode == temp->next->opcode)){
			if(temp->next->next->opcode == ADD || temp->next->next->opcode == SUB || temp->next->next->opcode == MUL)
				counter++;
		}
		temp = temp->next;
	}

	while(counter > 0) {
		//check this first so you don't allocate pointers to null
		if(ptr->next == NULL){
			ptr = head;
		}
		second = ptr->next;
		third = second->next;
		temp = head;
		counter = 0;
		//checking the operations for two loadis and an operation after
		while(temp != NULL){
			if(temp->opcode == LOADI && temp->opcode == temp->next->opcode)
				if(temp->next->next->opcode == ADD || temp->next->next->opcode == SUB || temp->next->next->opcode == MUL)	
					counter++;
			temp = temp->next;			
		}
		
		//printf("%d\n", counter);	



		//if one of the pairs have been found
		if (ptr->opcode == LOADI && second->opcode == LOADI) {
			switch (third->opcode) {
				//addition case
				case ADD:
					if ((ptr->field1 == third->field2 && second->field1 == third->field3) || (ptr->field1 == third->field3 && second->field1 == third->field2)) {
						//using the first node since it is a LOADI operation
						//we can replace it with all the information and get rid of the next two operations
						ptr->field1 = third->field1;
						//adding the constant from the first and the second register
						int sum = ptr->field2 + second->field2;
						//printf("sum:%d \n",sum);
						ptr->field2 = sum;
						ptr->next = third->next;
						third->next->prev = ptr;
						//frees the other parts of the linked lists
						free(second);
						free(third);
					}		
					break;
				//subtracting case
				case SUB:
					//order only matters in subtraction
					if (ptr->field1 == third->field2 && second->field1 == third->field3) {
						ptr->field1 = third->field1;
						int diff = ptr->field2 - second->field2;
						//printf("difference:%d \n", diff);	
						ptr->field2 = diff;
						ptr->next = third->next;
						third->next->prev = ptr;
						
						free(second);
						free(third);
					}
					if(ptr->field1 == third->field3 && second->field1 == third->field2){
						ptr->field1 = third->field1;
						int diff = second->field3 - ptr->field2;
						//printf("difference:%d \n", diff);
						ptr->field2 = diff;
						ptr->next = third->next;
						third->next->prev = ptr;
				
						free(second);
						free(third);
					}
					break;
				//multiply case
				case MUL:
					if ((ptr->field1 == third->field2 && second->field1 == third->field3) || (ptr->field1 == third->field3 && second->field1 == third->field2)) {
						ptr->field1 = third->field1;
						int product = ptr->field2 * second->field2;
						//printf("product:%d \n", product);
						ptr->field2 = product;
						ptr->next = third->next;
						third->next->prev = ptr;
						
						free(second);
						free(third);
					}		
					break;		
				default:
					break;
			}
		}
		ptr = ptr->next;
	}
 	PrintInstructionList(stdout, head);
 	DestroyInstructionList(head);
 	return EXIT_SUCCESS;
}

