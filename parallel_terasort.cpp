
#include "CommandLineArguments.h"

int main(int argc, char **argv)
{    
    //A time variable that is uded to compute the time taken by the program to execute
    time_t start = time(NULL);
    CommandLineArguments cmlarg; //object from the cmd class to start the program
    //parsing the cmd arguments that are written in the terminal
    if (cmlarg.parser(argc,argv))
    {
	//calling the boolean method (execute) to check whehter the args of the user are true or not
        if (!cmlarg.execute())
        {
            if ( strcmp(cmlarg.getErrorString(),"") != 0 )
                printf ("%s\n",cmlarg.getErrorString());//print the error to the user if the args in the terminal are wrong
        }        
    }
    else
    {
        if ( strcmp(cmlarg.getErrorString(),"") != 0 )
            printf ("%s",cmlarg.getErrorString());
        printf ("usage: %s --input-file <file-name> --output-file <file-name> --mappers <# of mappers> --reducers <# of reducers> --sample-size <# of records in sample>\n",argv[0]);
        exit(1);//exit the whole process if the user enterd wrong args to the terminal
   }
    time_t end = time(NULL);
    printf ("Total time: %lu\n",end-start);//print the time of the execution of the program
    
 
    return 0;
}
