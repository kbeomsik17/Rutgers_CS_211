#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Node {
	int data;
	struct Node* next;
};
struct Node* allocate(int);

struct Node* allocate(int dat){
	struct Node* temp=malloc(sizeof(struct Node));
	temp->data=dat;
	temp->next=0;
	return temp;
}

void freeNodes(struct Node* head){
	struct Node* temp;
	while(head!=0){
		temp=head;
		head=head->next;
		free(temp);
	}
}

struct Node* insertInOrder(struct Node* head, int a){
	struct Node* newNode=allocate(a);
	struct Node* current;
	if(head==0 || head->data > newNode->data){
		newNode->next=head;
		head=newNode;
	}
	else{
		current=head;
		while(current->next != 0 && current->next->data <= newNode->data){
			if(current->data==newNode->data){
				free(newNode);
				return head;
			}
			current=current->next;
		}
		if(current->data==newNode->data){
			free(newNode);
			return head;
		}
		newNode->next = current->next;
		current->next=newNode;
	}
	return head;
}

struct Node* deleteInOrder(struct Node* head, int a){
	struct Node* newNode=allocate(a);
	struct Node* current=head;
	if(head==0){
		head=0;
		free(newNode);
	}
	else if((head->next ==0 && head->data==newNode->data)){
		struct Node* temp=head;
		head=0;
		free(temp);
		free(newNode);
	}
	else{
		if(current->data==newNode->data){	
			struct Node* temp=head->next;
			free(head);
			free(newNode);
			return temp;
		}
		while(current->next != 0 && current->next->data !=newNode->data){
			current=current->next;
		}
		if(current->next==0 && current->data !=newNode->data){
			free(newNode);
			return head;
		}
		else if(current->next==0 && current->next->data ==newNode->data){
			struct Node* temp=current->next;
			current->next=0;
			free(newNode);
			free(temp);
		}
		else{
			struct Node* temp=current->next;
			current->next=current->next->next;
			free(newNode);
			free(temp);
		}
	}
	return head;
}

void printLinkedList(struct Node* head){
	struct Node* helper = head;
	while(helper != 0){
		printf("%d ", helper->data);
		helper=helper->next;
	}
	printf("\n");
}

int main(int argc, char* argv[argc+1]) {
	struct Node* head=0;
	char command[7];
	char insert[7]={'I', 'N', 'S', 'E', 'R', 'T', '\0'};
	int a=0;
	FILE* f=fopen(argv[1], "r");
	if(f==0){
		printf("error\n");
		return 0;
	}
	while(fscanf(f, "%s %i", command, &a) !=EOF){
		if(strcmp(command,insert)==0){
			head=insertInOrder(head, a);}
		else{
			head=deleteInOrder(head, a);}
		if(head==0){
			printf("EMPTY\n");
		}
		else{
			printLinkedList(head);
		}
	}
	fclose(f);
	freeNodes(head);
	return 0;
}
