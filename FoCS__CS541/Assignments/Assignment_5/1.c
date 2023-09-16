/******************************************************************************
* Executive M.Tech (AI & DSE)
* Subject: CS559 - Computer Systems Lab
* Name: Manjit Singh Duhan
* Addmission No: IITP001316
* Roll No : 2303res134
* email id: duhan.manjit@gmail.com
* Assignement 5
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PUSH_COMMAND_STRING             "PUSH "
#define POP_COMMAND_STRING              "POP"
#define ADD_COMMAND_STRING              "ADD"
#define SUB_COMMAND_STRING              "SUB"

/* Enums that will represent the instructions */
typedef enum {
    UNKNOWN = 0,
    PUSH,
    POP,
    ADD,
    SUB
}command_type;

/* The stack will be implemented using this node */
struct node {
    int data;
    struct node* next;
    struct node* prev;
};

/*This is the stack pointer. This will hold the topmost elemnet of the stack*/
struct node *top = NULL;

/******************************************************************************
* \brief  PUSH an node/element to the stack
*
* \param[in] value: the value that needs to be pushed to the stack
*
* \return : 0 if success otherwise non-Zero
* *****************************************************************************/
int push(int value)
{
    struct node* newNode = (struct node*)calloc(1, sizeof(struct node));
    if (newNode == NULL) {
        printf("ERR: Memory allocation failed. Cannot push %d\n", value);
        return -1;
    }

    newNode->data = value;
    newNode->next = NULL;
    newNode->prev = top;
    
    top = newNode;

    return 0;
}

/******************************************************************************
* \brief  POP an node/element from the stack
*
* \param[OUT] value: the value that is poped from the stack
*
* \return : 0 if success otherwise non-Zero
* *****************************************************************************/
int pop(int *value)
{
    if (top == NULL) {
        printf("ERR: Stack is empty. Cannot pop anything\n");
        return -1;
    }

    struct node* temp = top;
    
    *value = temp->data;

    if(temp->prev) {

        top = temp->prev;
        free(top->next);
        top->next = NULL;

    } else {

        /*This is the only node in stack. So, just free it*/
        free(top);
        top = NULL;
    }

    return 0;
}

/******************************************************************************
* \brief  parse the input given by user
*
* \param[in] input: the string entered by the user
* \param[out] cmd: the command entered by user. It is an enum
* \param[out] value: Valid oly when PUSH command is entered by user
*
* \return : 0 if success otherwise non-Zero
* *****************************************************************************/
int parse_input(char *input, command_type  *cmd, int *value)
{
    if (input == NULL || value == NULL) {
        return -1;
    }

    *value = 0;
    *cmd = UNKNOWN;

    //If \n is present at the end of input string then remove it.
    if (input[strlen(input) - 1] == '\n') {
        input[strlen(input) - 1] = '\0';
    }

    if (strncmp(input, PUSH_COMMAND_STRING, strlen(PUSH_COMMAND_STRING)) == 0) {
        int n = atoi(input + 5);

        if(n < -1000 || n > 1000) {
            printf("ERR: Invalid digit entered in PUSH command: %s\n", input+5);
            return -1;
        }

        *cmd = PUSH;
        *value = n;
    } 
    else if (strcmp(input, POP_COMMAND_STRING) == 0)
    {
        *cmd = POP;
    } 
    else if (strcmp(input, ADD_COMMAND_STRING) == 0)
    {
        *cmd = ADD;
    } 
    else if (strcmp(input, SUB_COMMAND_STRING) == 0)
    {
        *cmd = SUB;
    } else
    {
        return -1;
    }

    return 0;
}


int main()
{
    char input_string[15] = {0, };
    int number_of_instructions = 0;
    
    fgets(input_string, sizeof(input_string), stdin);

    number_of_instructions = atoi(input_string);
    if( (number_of_instructions < 1) || (number_of_instructions > 100)) {
       printf("\nERROR: Number of test case format is wrong. It should be < 1 & > 100. Exitting...");
       return -1; 
    }

    while(number_of_instructions--)
    {
        int value = 0;
        int retVal = 0;
        command_type cmd = UNKNOWN;
        
        fgets(input_string, sizeof(input_string), stdin);

        if (parse_input(input_string, &cmd, &value) == 0)
        {
            switch(cmd)
            {
                case PUSH:
                    {
                        if(push(value) != 0) {
                            printf("ERR: Error observed during PUSH operation\n");
                        }
                        break;
                    }
                case POP:
                    {
                        if(pop(&value) != 0) {
                            printf("ERR: Error observed during pop operation\n");
                        } else {
                            printf("%d\n", value);
                        }
                        break;
                    }
                case ADD:
                {
                    int a = 0, b = 0;

                    if ((pop(&a) == 0) && (pop(&b) == 0) ){
                        a += b;
                        if(push(a) != 0) {
                            printf("ERR: ADD Operation failed while pushing back ADD result\n");
                        }
                    } else {
                        printf("ERR: Stack does not have enough values for this operation. ADD Operation failed\n");
                    }
                    break;
                }

                case SUB:
                {
                    int a = 0, b = 0;
                    
                    if ((pop(&a) == 0) && (pop(&b) == 0) ){
                        b -= a;
                        if(push(b) != 0) {
                            printf("ERR: SUB Operation failed while pushing back SUB result\n");
                        }
                    } else {
                        printf("ERR: Stack does not have enouh values for this operation. SUB Operation failed\n");
                    }
                    break;
                }

                default :
                    printf("ERR: Unknown command entered. Skipping it...\n");
                break;
            }
        }
        else
        {
            printf("Unkown command\n");
        }
    }
    return 0;
}

