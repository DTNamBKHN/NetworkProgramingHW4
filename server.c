/*
	Simple udp server
*/
#include<stdio.h>	//printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUFLEN 512	//Max length of buffer
#define PORT 8888	//The port on which to listen for incoming data
typedef struct {
    char user_name[512];
    char password[512];
    int status;
} elementtype;
typedef struct node node;
typedef struct node {
	elementtype element;
	node *next;
} node;

typedef struct {
  node *root;
  node *cur;
  node *prev;
  node *tail;
} singleList;
singleList list;
//Declare function of linked list
void createSingleList(singleList *list);
node *makeNewNode(elementtype e);
node *insertEnd(singleList *list, elementtype e);
node *deleteHead(singleList *list);
node *deleteAtPosition(singleList *list, int position);
void printSingleList(singleList list);
//Declare function
int strcicmp(char const *a, char const *b);
void readDataFromFile(singleList *list);
int searchAccount(singleList list, char username[512]);
void alterDataOfFile(singleList list);
void blockAccount(singleList list,char username[512]);
int checkBlocked(singleList list, char username[512]);//if blocked, return 1
int checkCorrectPassword(singleList list, char username[512], char pass[512]);
int changePass(singleList accountList, char username[BUFLEN], char pass[BUFLEN], char new_pass[BUFLEN]);
int strcicmp(char const *a, char const *b);
int split_number_and_string(char *input, char *number,char *string);
//Implement function of linked list
void createSingleList(singleList *list)
{
    (*list).root = (*list).prev = (*list).cur = (*list).tail  = NULL;
}
node *makeNewNode(elementtype e)
{
    node *new = (node *)malloc(sizeof(node));
    new->element = e;
    new->next = NULL;
    return new;
}
node *insertEnd(singleList *list, elementtype e)
{
	node *new = makeNewNode(e);
	if((*list).root == NULL)
	{
        (*list).root = (*list).tail = new;
	}
	else
	{
		(*list).tail->next = new;
		(*list).tail = new;
	}
	return (*list).tail;
}
node *deleteHead(singleList * list)
{
    if((*list).root != NULL)
    {
        node *new = (*list).root;
		(*list).root = (*list).root->next;
		free(new);
	}
    return (*list).root;
}
node *deleteAtPosition(singleList *list, int position){
    if((*list).root == NULL)
    {
        return (*list).root;
    }
    (*list).cur = (*list).prev = (*list).root;
	//Delete head node
	if (position == 0){
        return deleteHead(list);
	}
	(*list).cur = (*list).prev = (*list).root;
	//store previous of to be deleted node
    for (int i = 0; (*list).cur != NULL && i < position - 1; i++)
    {
        (*list).prev = (*list).cur;
        (*list).cur = (*list).cur->next;
    }
    if ((*list).cur == NULL || (*list).cur->next == NULL)// If position is more than number of nodes
    {
        return (*list).cur;
    }
    if ((*list).cur->next->next == NULL){
        (*list).prev =( *list).cur;
        (*list).cur = (*list).cur->next;
        node *new = (*list).cur;
		(*list).cur = (*list).cur->next;
		free(new);
        (*list).tail = (*list).prev;
        (*list).tail->next = NULL;
        return (*list).tail;
    }
    //delete node at pos (next of pos-1)
    (*list).prev = (*list).cur;
    node *next = (*list).cur->next->next;
    free((*list).cur->next);
    (*list).prev->next = next;
    return (*list).cur;
}
void printSingleList(singleList list){
    list.cur = list.root;
	while(list.cur != NULL)
    {
        printf("%s %s %d\n", list.cur->element.user_name, list.cur->element.password, list.cur->element.status);
        list.cur = list.cur->next;
    }
    return;
}
void die(char *s)
{
	perror(s);
	exit(1);
}
//Implement function
void readDataFromFile(singleList *list){
    elementtype element;
    FILE *fp;
    fp = fopen("nguoidung.txt","r");
    char chr = getc(fp);
    int count_lines = 0;
    while (chr != EOF)
    {
        //Count whenever new line is encountered
        if (chr == '\n')
        {
            count_lines = count_lines + 1;
        }
        //take next character from file.
        chr = getc(fp);
    }
    fseek(fp, 0, SEEK_SET);
    for(int i = 0; i < count_lines; i++){
      fscanf(fp, "%s %s %d", element.user_name, element.password, &element.status);
      insertEnd(list, element);
    }
    fclose(fp);
}
int searchAccount(singleList list, char username[512]){
    list.cur = list.root;
    // if existed , return 1
    // else return 0
    while (list.cur != NULL)
    {
        if (strcicmp(list.cur->element.user_name, username) == 0)
            return 1;
        list.cur = list.cur->next;
    }
    return 0;
}
void alterDataOfFile(singleList list){
	FILE *fp;
    fp = fopen("nguoidung.txt","w");
    if (fp == NULL){
        printf("No file input\n");
        return;
    }
    list.cur = list.root;
	while(list.cur != NULL)
    {
        fputs(list.cur->element.user_name, fp);
        fputs(" ", fp);
        fputs(list.cur->element.password, fp);
        fputs(" ", fp);
        fprintf(fp, "%d", list.cur->element.status);
        fputs("\n", fp);
        list.cur = list.cur->next;
    }
    fclose(fp);
    return;
}
void blockAccount(singleList list,char username[512]){
	list.cur = list.root;
	while(list.cur != NULL){
		if (strcicmp(list.cur->element.user_name, username) == 0){
			list.cur->element.status = 0;
			return;
		}
		list.cur = list.cur->next;
	}
}
int checkBlocked(singleList list, char username[512]){
    list.cur = list.root;
	while(list.cur != NULL){
		if (strcicmp(list.cur->element.user_name, username) == 0){
			if(list.cur->element.status == 0){
				return 1;
			}
			else{
				return 0;
			}
		}
		list.cur = list.cur->next;
	}
}
int checkCorrectPassword(singleList list, char username[512], char pass[512]){
    list.cur = list.root;
	while(list.cur != NULL){
		if ((strcicmp(list.cur->element.user_name, username) == 0) && (strcicmp(list.cur->element.password, pass) == 0)){
			return 1;
		}
		list.cur = list.cur->next;
	}
	return 0;
}
int changePass(singleList accountList, char username[BUFLEN], char pass[BUFLEN], char new_pass[BUFLEN]){
	accountList.cur = accountList.root;
	while(accountList.cur != NULL){
		if ((strcicmp(accountList.cur->element.user_name, username) == 0) && (strcicmp(accountList.cur->element.password, pass) == 0)){
            strcpy(accountList.cur->element.password, new_pass);
            return 1;
		}
		accountList.cur = accountList.cur->next;
	}
	return 0;
}
int strcicmp(char const *a, char const *b){
    for (;; a++, b++) {
        int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
        if (d != 0 || !*a)
            return d;
	}
}
int split_number_and_string(char *input, char *number,char *string){
	// if number, copy to number
	// if character, copy to string
	int x = 0, y = 0;

	for(int i = 0; i < strlen(input); i++){
		if (input[i] == '\0') break;
		if(input[i] >= '0' && input[i] <= '9'){
    	    number[x] = input[i];
    	   	x++;
   		}
   		else if ((input[i] >= 'a' && input[i] <= 'z')||(input[i] >= 'A' && input[i] <= 'Z')){
   			string[y] = input[i];
   			y++;
   		}
   		else{
   			return 0;
   		}
   	}
   	number[x] = '\0'; 
	string[y] = '\0'; 
	return 1;
}
int main(void)
{
	FILE *fp;
    fp = fopen("nguoidung.txt","r");
	createSingleList(&list);
	readDataFromFile(&list);
	//UDP part
	struct sockaddr_in si_me, si_other;
	
	int s, i, slen = sizeof(si_other) , recv_len;
	char username[BUFLEN], pass[BUFLEN], new_pass[BUFLEN], 
	buf[BUFLEN], number[BUFLEN], string[BUFLEN];
	
	//create a UDP socket
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		die("socket");
	}
	
	// zero out the structure
	memset((char *) &si_me, 0, sizeof(si_me));
	
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//bind socket to port
	if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
	{
		die("bind");
	}
	
	//keep listening for data
	while(1)
	{
		printf("Waiting for data...");
		fflush(stdout);
		
		//try to receive username, this is a blocking call
		memset(username,'\0', BUFLEN);
		memset(number,'\0', BUFLEN);
		memset(string,'\0', BUFLEN);
		if ((recv_len = recvfrom(s, username, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
		{
			die("recvfrom()");
		}
		if (strcmp(username, "") == 0){
			printf("Closing...");
			close(s);
			return 0;
		}
		printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		printf("Data: %s\n" , username);
		if (searchAccount(list, username)){//check if account exist ?
			strcpy(buf, "Insert password\n");
			buf[strlen(buf) - 1] = '\0';
			if(sendto(s, buf, BUFLEN, 0, (struct sockaddr*) &si_other, slen) == -1){
				die("sendto()");
			}
			int wrong_pass_count = 0;
			while (1){
				memset(pass,'\0', BUFLEN);
				if ((recv_len = recvfrom(s, pass, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
				{
					die("recvfrom()");
				}
				printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
				printf("Data: %s\n" , pass);
				if(checkCorrectPassword(list, username, pass)){//check if pass is correct? 
					if(checkBlocked(list, username) != 1){//check if account is blocked?
						strcpy(buf, "OK\nEnter new password or 'bye' to sign out");
						buf[strlen(buf)] = '\0';
						if(sendto(s, buf, BUFLEN, 0, (struct sockaddr*) &si_other, slen) == -1){
							die("sendto()");
						}
						memset(new_pass,'\0', BUFLEN);
						if ((recv_len = recvfrom(s, new_pass, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
						{
							die("recvfrom()");
						}
						printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
						printf("Data: %s\n" , new_pass);
						if(strcmp(new_pass, "bye") == 0){
							strcpy(buf, "Goodbye ");
							strcat(buf, username);
							buf[strlen(buf)] = '\0';
							if(sendto(s, buf, BUFLEN, 0, (struct sockaddr*) &si_other, slen) == -1){
								die("sendto()");
							}
							break;
						}
						if(split_number_and_string(new_pass, number, string)){
							strcpy(buf, number);
							strcat(buf, string);
							buf[strlen(buf)] = '\0';
							if(sendto(s, buf, BUFLEN, 0, (struct sockaddr*) &si_other, slen) == -1){
								die("sendto()");
							}
							changePass(list, username, pass, new_pass);
							alterDataOfFile(list);
						}
						else{
							strcpy(buf, "Error");
							buf[strlen(buf)] = '\0';
							if(sendto(s, buf, BUFLEN, 0, (struct sockaddr*) &si_other, slen) == -1){
								die("sendto()");
							}
						}
						break;
					}
					else{
						strcpy(buf, "Account not ready\n");
						buf[strlen(buf) - 1] = '\0';
						if(sendto(s, buf, BUFLEN, 0, (struct sockaddr*) &si_other, slen) == -1){
							die("sendto()");
						}
						break;
					}
				}
				else{
					if (wrong_pass_count == 3){
						break;
					}
					strcpy(buf, "Not OK\n");
					buf[strlen(buf) - 1] = '\0';
					if(sendto(s, buf, BUFLEN, 0, (struct sockaddr*) &si_other, slen) == -1){
						die("sendto()");
					}
					wrong_pass_count++;
				}
			}
			if (wrong_pass_count == 3){
				strcpy(buf, "Account is blocked\n");
				buf[strlen(buf) - 1] = '\0';
				if(sendto(s, buf, BUFLEN, 0, (struct sockaddr*) &si_other, slen) == -1){
					die("sendto()");
				}
				blockAccount(list, username);
				alterDataOfFile(list);
				continue;
			}
		}
		else{
			strcpy(buf, "Cannot find account\n");
			buf[strlen(buf) - 1] = '\0';
            if(sendto(s, buf, BUFLEN, 0, (struct sockaddr*) &si_other, slen) == -1){
				die("sendto()");
			}
			continue;
        }
	}
	close(s);
	return 0;
}