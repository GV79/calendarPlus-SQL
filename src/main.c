#include "CalendarParser.c"

int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        printf("Only one input file argument.\n");
        exit(0);
    }
    Calendar * calendar = NULL;
    printf("%s\n", printError(createCalendar(argv[1], &calendar)));
/*
    //printf("%s\n", printError(createCalendar(argv[1], &calendar)));
    char * pointer = printCalendar(calendar);
if (pointer != NULL)
    printf("%s\n", pointer);
*/    
    deleteCalendar(calendar);
  //  free(pointer);

    return 0;
}
