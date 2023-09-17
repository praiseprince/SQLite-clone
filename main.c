#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
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

void close_input_buffer(InputBuffer* input_buffer){
    free(input_buffer->buffer); //since, getline() allocates memory for input_buffer->buffer we will need to free it up to prevent memory leak
    free(input_buffer);
}

int main(){
    InputBuffer* input_buffer=new_input_buffer();
    while(true){
        print_prompt();
        read_input(input_buffer);

        if(strcmp(input_buffer->buffer, ".exit")==0){
            close_input_buffer(input_buffer);
            exit(EXIT_SUCCESS);
        }
        else printf("Unrecognized command '%s'.\n", input_buffer->buffer);
    }
}