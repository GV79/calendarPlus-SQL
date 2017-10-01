#include "../include/LinkedListAPI.h"
#include "../include/CalendarParser.h"
#include <ctype.h>
//delete comments at the end

char * returnVersion(char * text)
{
    char textVerify[7];
    //char * version;
    float versionConverted = 0.0;
    int j, i;
    int length;
    j = 0;
//maybe malloc instead in case they put text smaller than version or check length and if < x then return NULL

    if (text[strlen(text)] != '\n')
    {
        text = strcat(text, "\n");
    }
    if (strlen(text) < 10)
    {
        return NULL;
    }

    for (i = 0; i < 7; i++)
    {
         textVerify[i] = toupper(text[i]);
    } i++; //skip : or ;

    if (strcmp(textVerify, "VERSION") == 0 && (text[7] == ';' || text[7] == ':'))
    {
        length = strlen(text) - strlen(textVerify) - 1; // - 1 for ; or :

        char * version = malloc(sizeof(char) * length + 1);
        while (text[i] != '\n')
        {
             version[j] = text[i];
             i++;
             j++;
        }


        if (sscanf(version, "%f", &versionConverted) == 1) //find some code
        {
            return version;
        }
        else
        {
            return NULL;
        }
    }
    return NULL;
}

//check for upper or lowercase for both functions
char * returnID(char * text)
{
    char textVerify[8];
    char * productID;
    int length = 0;
    int i, j;
    j = 0;

    if (text[strlen(text)] != '\n')
    {
        text = strcat(text, "\n");
    }

    if (strlen(text) < 9)
    {
        return NULL;
    }

    for (i = 0; i < 6; i++)
    {
        textVerify[i] = text[i];
    } i++;

    if (strcmp(textVerify, "PRODID") == 0 && (text[6] == ';' || text[6] == ':'))
    {
        length = strlen(text) - strlen(textVerify) - 1;
        productID = malloc(sizeof(char) * length + 1);
        while (text[i] != '\n')
        {
             productID[j] = text[i];
             i++;
             j++;
        }
        return productID;
    }
    return NULL;
}

int checkFile(char * fileName)
{
    if (fileName == NULL || fileName[0] == '\n' || fileName[0] == '\0')
    {
        return 1;
    }

    if (strlen(fileName) < 5)
    {
        return 1;
    }
    else
    {
        if (!(fileName[strlen(fileName)-1] == 's' && fileName[strlen(fileName)-2] == 'c' && fileName[strlen(fileName)-3] == 'i' && fileName[strlen(fileName)-4] == '.'))
        {
            return 1;
        }
    }

    return 0;
}

//need to move my functions into another c file and include it via #include
ErrorCode createCalendar(char* fileName, Calendar** obj)
{
    //obj = malloc(sizeof(Calendar *));
    (*obj) = malloc(sizeof(Calendar));
    //(*obj)->version = malloc(sizeof(float));

    if (checkFile(fileName) == 1)
    {
        free(*obj);
        free(obj);
        obj = NULL;
        return INV_FILE;
    }
    FILE * data = NULL;
    data = fopen(fileName, "r");
    int lineCount = 0;
    char dataText[10000];
    char * lineTrimmed = NULL;
    char * toFree = NULL;
    char * storedText[10000]; // free AFTER
//    char version[100];
    int versionCount = 0;
    int idCount = 0;
    int uidCount = 0;
    int dtCount = 0;
    int t;
    int i = 0;

    if (data != NULL)
    {
        while (fgets(dataText, 10000, data))
        {
            int p = 0;
            int k = 0;
            lineTrimmed = malloc(sizeof(char) * strlen(dataText) + 1);

            while (dataText[p] == ' ')
            {
                p++;
            }

            if (dataText[p] == ';' || dataText[p] == '\n')
            {
                continue;
            }

            while (dataText[p] != '\n')
            {
                while (dataText[p] == ' ')
                {
                    p++;
                    continue;
                }

                lineTrimmed[k] = dataText[p];
                k++;
                p++;
            }
            k = 0;
            p = 0;
            lineCount++;

            if (lineCount == 1)
            {
                while (i != strlen(lineTrimmed))
                {
                    lineTrimmed[i] = toupper(lineTrimmed[i]);
                    i++;
                } i = 0;

	        if (strcmp(lineTrimmed, "BEGIN:VCALENDAR") != 0)
                {
                    obj = NULL;
                    return INV_CAL;
                }
            }
            else
            {
                storedText[lineCount - 2] = malloc(sizeof(char) * strlen(lineTrimmed) + 1);
                strcpy(storedText[lineCount - 2], lineTrimmed);
                memset(lineTrimmed, 0, strlen(lineTrimmed));
                free(lineTrimmed);
            }
        }

        while (i != strlen(storedText[lineCount - 2]))
        {
            storedText[lineCount - 2][i] = toupper(storedText[lineCount - 2][i]);
            i++;
        } i = 0;
        if (strcmp(storedText[lineCount - 2], "END:VCALENDAR") != 0)
        {
            obj = NULL;
            return INV_CAL;
        }

        while (storedText[i] != '\0')
        {
printf("hello\n");
printf("%s each string\n", storedText[i]);

            char * originalString = calloc(strlen(storedText[i])+1, sizeof(char));
            strcpy(originalString, storedText[i]);
            char * stringCheck = strtok(storedText[i], ";:");
  	    if (stringCheck == NULL)
            {
                obj = NULL;
                return INV_CAL;
            }
            if (strcmp(stringCheck, "VERSION") == 0)
            {
                if (versionCount == 1)
                {
                    obj = NULL;
                    return DUP_VER;
                }
                toFree = returnVersion(originalString);
                if (toFree == NULL)
                {
                    obj = NULL;
                    return INV_VER; //also gotta check for duplicates
                }
                (*obj)->version = strtof(toFree, NULL);
                free(toFree);
                versionCount = 1;

            }
            else if (strcmp(stringCheck, "PRODID") == 0)
            {
                if (idCount == 1)
                {
                    obj = NULL;
                    return DUP_PRODID;
                }

                toFree = returnID(originalString);
                if (toFree == NULL)
                {
                    obj = NULL;
                    return INV_PRODID;
                }
                strcpy((*obj)->prodID, toFree);
                free(toFree);
                idCount = 1;

            }
            else if (strcmp(stringCheck, "BEGIN") == 0)
            {
                if (strcmp(originalString, "BEGIN:VEVENT") != 0)
                {
                    obj = NULL;
                    return INV_EVENT;
                }

                Event * event = malloc(sizeof(Event));
                while (strcmp(stringCheck, "END") != 0)
                {
                    i++;
                    free(originalString);
                    memset(stringCheck, 0, strlen(stringCheck));
                    originalString = calloc(strlen(storedText[i])+1, sizeof(char));
                    strcpy(originalString, storedText[i]);
                    stringCheck = strtok(storedText[i], ";:");

                    if (storedText[i] == '\0' || strcmp(storedText[i], "END:VCALENDAR") == 0)
                    {
                        obj = NULL;
                        return INV_EVENT;
                    }
                    else if (strcmp(stringCheck, "UID") == 0)
                    {
                        if (strlen(originalString) < 5)
                        {
                            obj = NULL;
                            return INV_EVENT;
                        }
                        if (!(originalString[3] == ':' || originalString[3] == ';'))
                        {
                            obj = NULL;
                            return INV_EVENT;
                        }
                        uidCount = 0;
                        char * split = strtok(originalString, ";:");
                        split = strtok(NULL, ";:");
                        printf("%s split %s stringCheck\n", split, stringCheck);
                        strcpy(event->UID, split);
                        (*obj)->event = event;
                    }
                    else if (strcmp(stringCheck, "DTSTAMP") == 0)
                    {

                    }

                    /*
            char * originalString = calloc(strlen(storedText[i])+1, sizeof(char));
            strcpy(originalString, storedText[i]);
            char * stringCheck = strtok(storedText[i], ";:");
                    */
                }
                continue;
            }
            free(originalString);
            memset(stringCheck, 0, strlen(stringCheck));
            i++;
        }
    }
    else
    {
        obj = NULL;
        return INV_FILE;
    }

    if (idCount == 0 || versionCount == 0) //and other required properties missing...etc.
    {
        obj = NULL;
        return INV_CAL;
    }
    fclose(data);
// have to free storedText loop
    return OK;
}

void deleteCalendar(Calendar* obj)
{
    return;
}

char* printCalendar(const Calendar* obj)
{
    return NULL;
}

char* printError(ErrorCode err)
{
    //ErrorCode array;
    printf("%d\n", err);
    return NULL;
}

