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

struct Node* pushNode(struct Node* head, int a){
	struct Node* newNode=allocate(a);
	newNode->next=head;
	head=newNode;
	return head;
}

struct Node* enqueueNode(struct Node* head, int a){
	struct Node* newNode=allocate(a);
	struct Node* current;
	current=head;
	if(head==0){
		head=newNode;
		return head;
	}
	while(current->next!=0){
		current=current->next;
	}
	current->next=newNode;
	return head;
}

struct Node* popNode(struct Node* head){
	if(head==0){
		return head;}
	struct Node* temp=head;
	temp=temp->next;
	free(head);
	return temp;
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
	char command[8];
	char enqueue[8]={'E', 'N', 'Q', 'U', 'E', 'U', 'E', '\0'};
	char push[5]={'P', 'U', 'S', 'H', '\0'};
	int a=0;
	FILE* f=fopen(argv[1], "r");
	if(f==0){
		printf("error\n");
		return 0;
	}
	while(fscanf(f, "%s %i", command, &a) !=EOF){
		if(strcmp(command,enqueue)==0){
			head=enqueueNode(head, a);}
		else if(strcmp(command,push)==0){
			head=pushNode(head, a);}
		else{
			head=popNode(head);}
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
