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
    Calendar * calendar = NULL;
    Calendar * new = NULL;
    char * temp;
    char * tempTwo = malloc(sizeof(char) * 100);
    char userInput[1000];
    char date[9];
    char time[7];
    char timeVerify[2];
    char * pointer= NULL;
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
                memset(tempTwo, 0, sizeof(char));
                strcpy(tempTwo, printError(createCalendar(argv[1], &calendar)));
                printf("%s\n", tempTwo);
                 memset(tempTwo, 0, sizeof(char));
                strcpy(tempTwo, printError(validateCalendar(calendar)));
                printf("%s\n", tempTwo);
                printf("writing\n");
      	        memset(tempTwo, 0, sizeof(char));
                strcpy(tempTwo, printError(writeCalendar("blip.ics", calendar)));
                printf("%s\n", tempTwo);

                break;
            case 2:
                if (calendar != NULL)
                {
                    temp = printCalendar(calendar);
                    printf("%s\n", temp);
                    free(temp);
                }
                break;
            case 3:
                //for now test validateCalendar here and do other shit
                if (new != NULL)
                {
                    free(new);
                }
                int z = 0;
                memset(date, 0, sizeof(char));
                memset(time, 0, sizeof(char));
                memset(userInput, 0, sizeof(char));
                memset(timeVerify, 0, sizeof(char));
                new = malloc(sizeof(Calendar));
                new->properties = initializeList(NULL, NULL, NULL);
                new->events = initializeList(NULL, NULL, NULL);
                Event * event = malloc(sizeof(Event));
                event->properties = initializeList(NULL, NULL, NULL);
                event->alarms = initializeList(NULL, NULL, NULL);
                Alarm * alarm = malloc(sizeof(Alarm));
                alarm->properties = initializeList(NULL, NULL, NULL);
                alarm->trigger = malloc(sizeof(char) * 100);

                printf("Creating a Calendar object from user input (assuming 1 event and 1 alarm): \n");
                printf("Enter the calendar version: \n");
                getchar();
                fgets(userInput, 100, stdin);           
                userInput[strlen(userInput)-1] = '\0';
                new->version = atof(userInput);
                memset(userInput, 0, sizeof(char));
                printf("Enter the calendar prodID: \n");
                fgets(userInput, 100, stdin);           
                userInput[strlen(userInput)-1] = '\0';
                strcpy(new->prodID, userInput);
                memset(userInput, 0, sizeof(char));
                printf("Enter the event UID: \n");
                fgets(userInput, 100, stdin);           
                userInput[strlen(userInput)-1] = '\0';
                strcpy(event->UID, userInput);
                memset(userInput, 0, sizeof(char));
                printf("Enter the alarm action for the event: \n");
                fgets(userInput, 100, stdin);           
                userInput[strlen(userInput)-1] = '\0';
                strcpy(alarm->action, userInput);
                memset(userInput, 0, sizeof(char));
                printf("Enter the alarm trigger for the event: \n");
                fgets(userInput, 100, stdin);           
                userInput[strlen(userInput)-1] = '\0';
                strcpy(alarm->trigger, userInput);
                memset(userInput, 0, sizeof(char));

                printf("Enter the event creationDateTime: \n");
                fgets(userInput, 100, stdin);
                userInput[strlen(userInput)-1] = '\0';

                printf("Verifying Calendar Object Status: \n");
                if (!(strlen(userInput) == 15 || strlen(userInput) == 16))
                {
                    printf("INV_CAL\n");
                    memset(tempTwo, 0, sizeof(char));
                    free(alarm->trigger);
                    free(alarm);
                    free(event);
                    free(new);
                    new = NULL;
                    break;
                }

                if (userInput[8] != 'T')
                {
                    printf("INV_CAL\n");
                    memset(tempTwo, 0, sizeof(char));
                    free(alarm->trigger);
                    free(alarm);
                    free(event);
                    free(new);
                    new = NULL;
                    break;
                }

                for (z = 0; z < 8; z++)
                {
                    date[z] = userInput[z];
                } date[z] = '\0';
                
                for (z = 0; z < 6; z++)
                {
                    time[z] = userInput[z+9];
                } time[z] = '\0';

                if (strlen(userInput) == 16)
                {
                    if (userInput[15] == 'Z')
                    {
                        timeVerify[0] = '1';
                    }
                } timeVerify[1] = '\0';
                insertBack(&event->alarms, alarm);
                strcpy(event->creationDateTime.date, date);
                strcpy(event->creationDateTime.time, time);
                
                if (strcmp(timeVerify, "1") == 0)
                {
                      event->creationDateTime.UTC = 1;
                }
                else
                {
                      event->creationDateTime.UTC = 0;
                }
                insertBack(&new->events, event);
                memset(tempTwo, 0, sizeof(char));
                strcpy(tempTwo, printError(validateCalendar(new)));
                printf("%s\n", tempTwo);
                pointer = alarm->trigger;
                break;
            case 4:
                printf("Enter filename with extension .ics: \n");
                scanf("%s", userInput);
                printf("%s\n", printError(writeCalendar(userInput, new)));
                break;
            case 5:
                printf("Thanks for using the program.\n");
                //deleteCalendar(calendar);

                if (calendar != NULL)
                {
                   // if (strcmp(printError(createCalendar(argv[1], &calendar)), "OK") == 0)
                    if (strcmp(tempTwo, "OK") == 0)
                    {
                        deleteCalendar(calendar);
                    }
                }

                break;
            default:
                printf("Program ran into error.\n");
                deleteCalendar(calendar);
                break;
        }
    }

    if (new != NULL)
    {
        free(pointer);
        free(new->events.head);
        free(new);
        new = NULL;
    }

    free(tempTwo);
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
                printf("Error: can only enter one of the following digits: '1', '2', '3', '4', '5'. Please try again:\n");
                return 0;
        }
        else
        {
            input = atoi(command);
            if (command[0] == '0' || input > 5)
            {
                printf("Error: can only enter one of the following digits: '1', '2', '3', '4', '5'. Please try again:\n");
                return 0;
            }
        }
    }
    return 1;
}

