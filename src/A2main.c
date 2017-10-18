#include "CalendarParser.h"
#include "ctype.h"

int verifyCommand(char * command);

int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        printf("Must have one argument for calendar file. Please re-run program with a file argument.\n");
        exit(0);
    }
    if (argv == NULL)
    {
        printf("File not valid. Please re-run program with proper file\n");
        exit(0);
    }

    printf("Welcome to the Calendar program by Giavinh Lam.\n");
    printf("Please select one of the following options by entering in the corresponding number\n");

    char command[1000];
    int convertString = 0;
    int errorChecking = 0;
    Calendar * calendar;
    Calendar * new = NULL;
    char * temp;
    while (convertString != 5)
    {
        while (errorChecking == 0)
        {
            printf("1. Read in iCalendar file argument using createCalendar. \n"); // shud make sure this is done b4 2
            printf("2. Display file to screen using printCalendar.\n");
            printf("3. Create a new Calendar object.\n");
            printf("4. Save the new Calendar object to a file using writeCalendar.\n");
            printf("5. Exit\n");
            scanf("%s", command);
            if (verifyCommand(command) == 1)
            {
                errorChecking = 1;
            }
            else
            {
                continue;
            }
        }
        errorChecking = 0;
        convertString = atoi(command);
        switch(convertString)
        {
            case 1:
                printf("%s\n", printError(createCalendar(argv[1], &calendar)));
                break;
            case 2:
                temp = printCalendar(calendar);
                printf("%s\n", temp);
                free(temp);
                break;
            case 3:
                //how does this work?? UI?
                break;
            case 4:
                printf("%s\n", printError(writeCalendar("newFile", new)));
                break;
            case 5:
                printf("Thanks for using the program.\n");
                deleteCalendar(calendar);
                exit(0);
            default:
                printf("Program ran into error.\n");
                deleteCalendar(calendar);
                exit(0);
        }
    }

    return 0;
}

int verifyCommand(char * command)
{
    int input = 0;
    int i = 0;
    for (i = 0; i < strlen(command); i++)
    {
        if (!isdigit(command[i]))
        {
                printf("Error: can only enter one of the following digits: '1', '2', '3', '4', '5', '6', '7'. Please try again:\n");
                return 0;
        }
        else
        {
            input = atoi(command);
            if (command[0] == '0' || input > 5)
            {
                printf("Error: can only enter one of the following digits: '1', '2', '3', '4', '5', '6', '7'. Please try again:\n");
                return 0;
            }
        }
    }
    return 1;
}

