#include "CalendarParser.h"

//delete comments at the end

char * returnID(char * text)
{
    char textVerify[8];
    char * version;
    float versionConverted = 0.0;
    int j, i;
    int length;
    j = 0;

    text = strcat(text, "\n");
    for (i = 0; i < 8; i++)
    {
         textVerify[i] = text[i];
    }

    if (strcmp(textVerify, "VERSION:") == 0)
    {
        length = strlen(text) - strlen(textVerify);
        version = malloc(sizeof(char) * length + 1);
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

ErrorCode createCalendar(char* fileName, Calendar** obj)
{
    obj = malloc(sizeof(Calendar*));
    *obj = malloc(sizeof(Calendar*));
    //(*obj)->version = malloc(sizeof(float);

    if (fileName == NULL || fileName[0] == '\n' || fileName[0] == '0')
    {
        //free(obj);
        obj = NULL;
        return INV_FILE;
    }
    FILE * data = NULL;
    data = fopen(fileName, "r");
    int i = 0; // letter counter for file lines
    int j = 0; // actual start of array in case of comments that are indented
    int k = 0; // line counter
    int l = 0; // letter counter for new 2D char array
    char dataText[10000];
    char * storedText[1000]; // free AFTER
    char version[100];

    int t;
    for (t=0; t < 1000; t++)
         storedText[t] = (char *)malloc(100 * sizeof(char));

    if (data != NULL)
    {
        while (fgets(dataText, 10000, data))
        {
//printf("%s\n", dataText);
            while (dataText[i] != '\n')
            {
                if (dataText[i] == ' ')
                {
                    i++;
                    j = i;
                    continue;
                }

                if (dataText[j] == ';' || dataText[0] == ';')
                {
                    i = 0;
                    j = 0;
//printf("invalid\n");
                    break;
                }
                else
                {
                    //dataText[strlen(dataText)-1] = '\0';
//printf("%c\n", dataText[i]);
                    storedText[k][l] = dataText[i];
//printf("next %c\n", storedText[0][l]);
                    l++;
                }
                i++;
            }
            memset(dataText,0,strlen(dataText));
            i = 0;
            j = 0;
            l = 0;
            if (storedText[k][0] != '\0')
            {
                k++;
            }
        }

        if (strcmp(storedText[0], "BEGIN:VCALENDAR") != 0) {
            return INV_CAL;
        }
        if (returnVersion(storedText[1]) == NULL)
        {
            return INV_VER; //also gotta check for duplicates
        }
        strcpy(version, returnVersion(storedText[1]));
        free(returnVersion(storedText[1]));
        (*obj)->version = strtof(version, NULL);

        if (returnID(storedText[2]) == NULL)
        {

        }
    }
    else
    {
        return INV_FILE;
    }

    fclose(data);
    return INV_CAL;
}

void deleteCalendar(Calendar* obj)
{
    return;
}

char* printCalendar(const Calendar* obj)
{
    return NULL;
}

const char* printError(ErrorCode err)
{
    return NULL;
}

