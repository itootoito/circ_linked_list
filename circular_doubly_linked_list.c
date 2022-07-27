#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


//============ STRUCTURE DECLARATION =================
struct node 
{
	struct node* prev;
	int data;
	struct node* next;
};

//=============== FUNCTION PROTOTYPES =================

//--- misc functions ---
int inputVerInt(char prompt[]);

//--- visualization functions ---
void clear();
int list_len(struct node* tail);
void print_list(struct node* tail, int pos_on);

//--- node deletion functions ---
struct node * delete_pos(struct node *tail, int pos);
struct node* delete_last(struct node *tail);
struct node* delete_first(struct node *tail);

// --- node creation functions ---
struct node* add_aftr(struct node* tail, int data, int pos);
struct node* add_end(struct node* tail, int data);
struct node* add_empty(int data);
struct node* add_beg(struct node* tail, int data);

//======================= main =======================

int main()
{
	struct node* tail = NULL;

	
	tail = add_empty(45);
	//tail = add_beg(tail, 34);
	//tail = add_end(tail, 56);
	//tail = add_aftr(tail, 21, 0); 
	/*
	print_list(tail);
	printf("list length = %d\n", list_len(tail) );

	//tail = delete_first(tail);
	//tail = delete_last(tail);
	//tail = delete_pos(tail, 1);

	print_list(tail);
	printf("list length = %d\n", list_len(tail) );
	*/

	int choice = 0;
	int choice2 = 0;
	int on = 1; 
	int data_temp;
	int pos_temp;
	

		clear();
	do{
		printf("=====================================\n");
		printf("	circular linked list\n ");
		printf("=====================================\n");
		printf("1. print list\n");
		printf("2. add node\n");
		printf("3. remove node\n");
		printf("0. quit\n");

		on = inputVerInt("choose option: \n");
		
		switch(on)
		{
			case 1:
				clear();
				print_list(tail, 0);
				break;
			
			case 2:
				clear();
			       	printf("--------------------\n");
				printf("    choose method\n");
			       	printf("--------------------\n");
				printf("1. add at beginning\n");
				printf("2. add after position\n");
				printf("3. add at end\n");
				choice2 = inputVerInt("chose option: \n");

				switch(choice2)
				{
					case 1: 
						data_temp = inputVerInt("data: ");
						tail = add_beg(tail, data_temp);
						break;

					case 2:
						clear();
						print_list(tail, 1);
						
						pos_temp = inputVerInt("position: ");
						data_temp = inputVerInt("data: ");

						if(pos_temp == 1) { 
							add_beg(tail, data_temp);
						}else
						{
							tail = add_aftr(tail, data_temp, pos_temp - 1);
						}
						break;

					case 3:
						data_temp = inputVerInt("data: ");
						tail = add_end(tail, data_temp);
						break;
				}
				break;
	
			case 3:
				clear();
			       	printf("--------------------\n");
				printf("    choose method\n");
			       	printf("--------------------\n");
				printf("1. remove at beginning\n");
				printf("2. remove by position\n");
				printf("3. remove at end\n");
				choice2 = inputVerInt("chose option: \n");
				
				switch(choice2)
				{
					case 1:
						tail = delete_first(tail);
						break;
					case 2:
						clear();
						print_list(tail, 1);

						pos_temp = inputVerInt("position: ");
						tail = delete_pos(tail, pos_temp + 1);
						break;
					case 3:
						tail = delete_last(tail);
						break;
				}
				break;
			
			case 0: 
				printf("exiting...\n");
				break;

			default:
				printf("ERROR: invalid input\n");	
		}
		
		

	} while( on != 0);

	return 0;
}

//++++++++++++++++++++++++++++++++ FUNCTIONS ++++++++++++++++++++++++++++++++++++

//=================== MISC FUNCTIONS ==================
int inputVerInt(char prompt[])
{
	
	int exit = 0; //loop controller
	char scan[100]; //dirty input
	int scanFinal; //cleaned input
	int ver = 1; //verification variable
	
	//main loop
	do
	{
		printf("%s", prompt);//prompt print
		fgets(scan, 100, stdin); //dirty user input
		
		//sanitization
		if(atoi(scan) != 0 || strcmp(scan, "0\n") == 0 || strcmp(scan, "0.0\n") == 0) //user input accepted(is an int)
		{
			scanFinal = atoi(scan); //transforms the string into an int
			exit = 1; //ends loop
			
		}else //user input rejected
		{
			printf("Invalid input! Please try again!\n");
		}
		
	}while(exit != 1); 
	
	return scanFinal; //returns the verified input
}


//=============== NODE CREATION =====================

//create first node of list
struct node* add_empty(int data)
{
	struct node* temp = malloc(sizeof(struct node));
	temp->prev = temp;
	temp->data = data;
	temp->next = temp;
	return temp;
}

//add at beginning of list
struct node* add_beg(struct node* tail, int data)
{
	struct node* newptr = add_empty(data);
	if(tail == NULL)
	{
		return newptr;
	}else
	{
		struct node* temp = tail->next;
		
		newptr->prev = tail;
		newptr->next = temp;
		temp->prev = newptr;
		tail->next = newptr;
		return tail;
	}
}

struct node* add_end(struct node* tail, int data)
{
	struct node* newptr = add_empty(data);

	if(tail == NULL)
	{
		return newptr;
	}else
	{
		struct node *temp = tail->next; //temp becomes the first node
		newptr->next = temp; //newnode next points to first node(circle)
		newptr->prev = tail; //newnode prev points to last node
		tail->next = newptr; //tail next points to newnode
		temp->prev = newptr; //first node points prev to newnode(circle)
		tail = newptr; //newnode is now the last node(tail) 
		return tail;
	}
}

struct node* add_aftr(struct node* tail, int data, int pos)
{
	struct node* newptr = add_empty(data);

	if(tail == NULL)
		return newptr;

	//contigencies for position overflowing length or with negative value
	if(list_len(tail) < pos ){
		printf("ERROR: cannot add node at invalid position %d (pos bigger than list length)\n", pos);
		return tail;
	}else if(pos < 0) {
		printf("ERROR: cannot add node at invalid position %d (pos smaller than minimum length)\n", pos);
		return tail;
	}


	struct node* temp = tail->next;

	//navigates temp to position where newnode will be placed after
	while(pos > 1)
	{
		temp = temp->next; //temp points to frst node in list(consider it the last node in this operation, remember its a circular list)
		pos--; //decremented until temp becomes the node in desired position
	}

	//new node is fitted behind temp:
	
		//new node prev points to temp
	newptr->prev = temp;
		//newnode next points to temp next(node after temp that already exists)
	newptr->next = temp->next; 
		//node that was in front of temp now points to new node in its prev
	temp->next->prev = newptr;
		//temp next points to the new node 
	temp->next = newptr; 
		//if the temp was tail, tail will have to be updated
	if(temp == tail)
	{
		tail = tail->next;
	}	
	
	return tail; //new tail returned.
}


//=============== NODE DELETION =====================

struct node* delete_first(struct node *tail)
{
	if(tail == NULL)
	{
		return tail;
	}

	struct node* temp = tail->next;

	//if theres only one node, simply free its memory and clear the address
	if(temp == tail)
	{
		free(tail);
		tail = NULL;
		return tail;
	}


	//otherwise just fix the addresses of the last node and the second node
	tail->next = temp->next;
	temp->next->prev = tail;
	free(temp);
	
	return tail;
}

struct node* delete_last(struct node *tail)
{
	//if list is empty
	if(tail == NULL)
	{
		return tail;
	}else if(list_len(tail)== 1)
	{
		printf("here\n");
		return delete_first(tail); //dont know why but smt was causing a seg fault down there
		
	}

	struct node* temp = tail->prev;
	
	//adjust the addresses of the second last node and free the memory
	//temp = tail->prev;
	temp->next = tail->next;
	tail->next->prev = temp;
	free(tail);
	tail = temp;
	return tail;
}

struct node * delete_pos(struct node *tail, int pos)
{
	
	//contigencies for position overflowing length or with negative value
	if(list_len(tail) < pos ){
		printf("ERROR: cannot remove node at invalid position %d (pos bigger than list length)\n", pos);
		return tail;
	}else if(pos <= 0) {
		printf("ERROR: cannot remove node at invalid position %d (pos smaller than minimum length)\n", pos);
		return tail;
	}

	
	//if node is only one in list
	if(tail->next == tail)
	{
		free(tail);
		tail = NULL;
		return tail;	
	}

	//node 
	struct node *temp = tail->next;

	//traversal to desired position 
	while(pos > 1)
	{
		temp = temp->next;
		pos--;
	}

	//node before position
	struct node *temp2 = temp->prev;

	//update to address
	temp2->next = temp->next;
	temp->next->prev = temp2;
	free(temp);

	//if node deleted is the last one, tail needs to point to second last
	if(temp == tail)
	{
		tail = temp2;
	}

	return tail;
}


//=========== VISUALIZATION FUNCTIONS ===============
void print_list(struct node* tail, int pos_on)
{
	if(tail == NULL){
		printf("list has no elements!\n");
	}else {
		struct node* temp = tail->next;
		int count = 0;

		do
		{
			if(pos_on == 1) {
				printf( " %d    %d\n      |\n", count, temp->data);	
				count++; 
			}else {
				printf(" %d\n |\n", temp->data);

			}
			temp = temp->next;

		} while(temp != tail->next);
	}
	printf("\n");
}	

int list_len(struct node* tail)
{
	if(tail == NULL){
		return 0;
	}else 
	{
		struct node* temp = tail->next;
		int node_count = 0;

		do
		{
			temp = temp->next;
			node_count++;
		} while (temp != tail->next);

		return node_count;
	}	
	
}

void clear()
{
	system("clear"); //clear screen	
}