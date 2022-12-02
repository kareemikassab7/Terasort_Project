#include "CommandLineArguments.h"


CommandLineArguments::CommandLineArguments()
{
    //A constructor setting the data members to zeros
    memset(input_file_name,0,COMMAND_LINE_ARGUMENT_MAX_SIZE); 
    memset(output_file_name,0,COMMAND_LINE_ARGUMENT_MAX_SIZE);
    memset(error_string,0,ERROR_STRING_MAX_SIZE);
    mappers = 0;
    reducers = 0;
    sample_size = 0;    
}

//boolean function that parses the cmd arguments and returns true if the arguments were passed in the right way
//returns false and the print the right way to enter the parameters if the users entered them wrong
bool CommandLineArguments::parser(int argc,char ** argv)
{
    if ( argc %2 == 0 || argc < 2) return false;
    else
    {
        for ( uint8_t i = 1 ; i < argc ; i +=2)
        {
            if ( strcmp(argv[i],"--input-file") == 0)
                strncpy(input_file_name,argv[i+1],COMMAND_LINE_ARGUMENT_MAX_SIZE-1);
            else if ( strcmp(argv[i],"--output-file") == 0)
                strncpy(output_file_name,argv[i+1],COMMAND_LINE_ARGUMENT_MAX_SIZE-1);
            else if ( strcmp(argv[i],"--mappers") == 0)
            {
                mappers =atoi(argv[i+1]);
            }
            else if ( strcmp(argv[i],"--reducers") == 0)
            {
                reducers =atoi(argv[i+1]);
            }
            else if ( strcmp(argv[i],"--sample-size") == 0)
            {
                sample_size =atoi(argv[i+1]);
            }
            else
            {
                sprintf(error_string,"undefined parameter: %s\n",argv[i]);
                return false;
            }
        }
        if ( strcmp (input_file_name,"") == 0 || strcmp (output_file_name,"") == 0 || 
                mappers == 0 || reducers == 0 | sample_size == 0 ) return false;
        else return true;
    }
}
bool CommandLineArguments::execute()
{
    //execute function that starts the whole program when it takes the right arguments
    //this method is gonna be invoked from the main function
    TeraSort <TeraSortItem,teraitem_r> * teraSort = 
            new TeraSort <TeraSortItem,teraitem_r> (input_file_name,output_file_name,mappers,reducers,sample_size);
    teraSort->execute();
    //destroying the terasort pointer after the program finishes
    delete(teraSort);
    //returns true when it finishes working
    return true;
}
char * CommandLineArguments::getErrorString()
{
    //return the string that should be printed for the user if he enterd the cmd arguments wrong
    return error_string;
}
CommandLineArguments::~CommandLineArguments()
{
    //empty destructor
}
