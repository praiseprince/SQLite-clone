#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{ //this enum will serve as the result for meta command check
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
}MetaCommandResult;

typedef enum{ //this enum will serve as the result for statement check
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
}PrepareResult;

typedef enum{ //this enum will tell what type of statement the user has passed
    STATEMENT_INSERT,
    STATEMENT_SELECT
}StatementType;

typedef struct{ //this struct is used to improve the readability of the code
    StatementType type;
}Statement;

typedef struct{ //this struct will create an ADT for the buffer
    char* buffer;
    size_t buffer_length; //buffer length tells the length of the buffer i.e it also includes the '/0' in the end
    ssize_t input_length; //input length tells the length of the string, ssize_t means signed size_t
} InputBuffer;

InputBuffer* new_input_buffer(){ //this function creates a new InputBuffer
    InputBuffer* input_buffer=(InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer=NULL;
    input_buffer->buffer_length=0;
    input_buffer->input_length=0;

    return input_buffer;
}

void print_prompt() {printf("db > ");} //this function prints the standard teminal prompt

void read_input(InputBuffer* input_buffer){ //this function reads an input from the user
    /*the getline() function takes in three arguements:
    1. Address of the variable that will store the buffer: 
    This argument is a pointer to a character pointer (char**) 
    and is used to store the address of the buffer where the input line will be stored. 
    It allows getline() to allocate memory dynamically for the input line. 
    If the buffer is initially NULL or has insufficient space, getline() will allocate memory as needed.
    
    2. Address of the variable that will store the buffer length: This argument is a pointer to a size_t variable (size_t*) 
    and is used to store the buffer length. 
    It represents the amount of memory available for storing the input line.
    
    3. Stream from which to take input: in this case, stdin
    
    getline() returns the number of bytes read*/

    ssize_t bytes_read=getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
    if(bytes_read<=0){
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    input_buffer->input_length=bytes_read-1;
    input_buffer->buffer[bytes_read-1]=0; //the last element in the array buffer[] i.e buffer[bytes_read-1] will be '\n' so, we will get a trailing line and to prevent it we will need to change it to a null terminator
}

void close_input_buffer(InputBuffer* input_buffer){ //this function frees up the allocated memory
    free(input_buffer->buffer); //since, getline() allocates memory for input_buffer->buffer we will need to free it up to prevent memory leak
    free(input_buffer);
}

MetaCommandResult do_meta_command(InputBuffer* input_buffer){ //this function executes the meta command
    if(strcmp(input_buffer->buffer, ".exit")==0){
            close_input_buffer(input_buffer);
            exit(EXIT_SUCCESS);
        }
    else{
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement){ //this function checks whether the statement passed by the user is valid or not and if it's valid then it determines the type of statement
    if(strncmp(input_buffer->buffer, "insert", 6)==0){
        statement->type=STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if(strncmp(input_buffer->buffer, "select", 6)==0){
        statement->type=STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement){ //this function executes the statement provided by the user
    switch(statement->type){
        case(STATEMENT_INSERT):
        printf("This is where we would do an insert.\n");
        break;
        case(STATEMENT_SELECT):
        printf("This is where we would do a select.\n");
        break;
    }
}

int main(){
    InputBuffer* input_buffer=new_input_buffer();
    while(true){
        print_prompt();
        read_input(input_buffer);
        
        /*Personal Notes: In this code, at this point, using 'continue' or 'break' statement in all of the switch cases would not matter as
        it is inside a while loop, I think this is for future, when more features and commands are added*/

        if(input_buffer->buffer[0]=='.'){ //this checks if the buffer is supposed to be a meta command or a statement
            switch(do_meta_command(input_buffer)){
                case(META_COMMAND_SUCCESS):
                continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                printf("Unrecognized command '%s'\n", input_buffer->buffer);
                continue;
            }
        }

        Statement statement; //this stores the statement type, a struct is used to improve the readability
        switch(prepare_statement(input_buffer, &statement)){
            case(PREPARE_SUCCESS):
            break;
            case(PREPARE_UNRECOGNIZED_STATEMENT):
            printf("Unrecognized keyword at start of '%s'.\n",input_buffer->buffer);
            continue;
        }
        execute_statement(&statement); //if the statement is valid then this executes the sattement
        printf("Executed.\n");
    }
}