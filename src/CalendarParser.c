//#include "../include/LinkedListAPI.h"
#include "LinkedListAPI.c"
#include "../include/CalendarParser.h"
#include <ctype.h>
//delete comments at the end

char* printFunc(void *toBePrinted){
	
	return (char*)toBePrinted;
}

//Comparing strings is done by strcmp
int compareFunc(const void *first, const void *second){
	
	return strcmp((char*)first, (char*)second);
}

//Freeing a string is also straightforward
void deleteFunc(void *toBeDeleted){
	
	free(toBeDeleted);
}

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

char * returnDT(char * text)
{
    char * returnAll;
    char date[9];
    char time[9];
    char timeVerify[2];

    if (strlen(text) != 24)
    {
        return NULL;
    }

    if (!(text[7] == ':' || text[7] == ';')) //might not be necessary
    {
        return NULL;
    }

    int i = 0;
    for (i = 0; i != strlen(text); i++)
    {
        if (text[i] == '#')
            return NULL;
    }

    for (i = 0; i < 8; i++)
    {
        date[i] = text[i+8];
    }

    if (text[16] != 'T')
        return NULL;

    for (i = 0; i < 6; i++)
    {
        time[i] = text[i+17];
    }

    if (text[23] == 'Z') //ask someone bout this
    {
        timeVerify[0] = '1';
    }
    else
    {
        timeVerify[0] = '0';
    }
    // if (dtCount == 1)
    // {
     //    obj = NULL;
      //             return INV_CREATEDT;
       //                 } // do this in the function calling this function

    returnAll = malloc(sizeof(char) * 19 + 1);
    strcpy(returnAll, date);
    returnAll = strcat(returnAll, "#");
    returnAll = strcat(returnAll, time);
    returnAll = strcat(returnAll, "#");
    returnAll = strcat(returnAll, timeVerify);
    return returnAll;
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
    int eventCount = 0;
    int triggerCount = 0;
    int actionCount = 0;
    int t = 0;
    int i = 0;
    int z = 0;

    if (data != NULL)
    {
        while (fgets(dataText, 10000, data))
        {
            int p = 0;
            int k = 0;
            lineTrimmed = malloc(sizeof(char) * strlen(dataText) + 1);
            if (lineTrimmed == NULL)
            {
                return OTHER_ERROR;
            }

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
            char * originalString = calloc(strlen(storedText[i])+1, sizeof(char));
            //char * uppercaseString = calloc(strlen(storedText[i]+1, sizeof(char));
            strcpy(originalString, storedText[i]);
            char * stringCheck = strtok(storedText[i], ";:");
  	    if (stringCheck == NULL)
            {
                obj = NULL;
                return INV_CAL;
            }

            while (t != strlen(stringCheck))
            {
                stringCheck[t] = toupper(stringCheck[t]);
                t++;
            } t = 0;

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
                while (t != strlen(originalString))
                {
                    originalString[t] = toupper(originalString[t]);
                    t++;
                } t = 0;

                if (strcmp(originalString, "BEGIN:VEVENT") != 0 || eventCount == 1)
                {
                    obj = NULL;
                    return INV_EVENT;
                }

                Event * event = malloc(sizeof(Event));
                (*obj)->event = event;
                event->properties = initializeList(&printFunc, &deleteFunc, &compareFunc);
                event->alarms = initializeList(&printFunc, &deleteFunc, &compareFunc); //maybe put higher?
//                alarm->properties = initializeList(&printFunc, &deleteFunc, &compareFunc); // maybe put higher??

                eventCount = 1;
                while (strcmp(originalString, "END:VEVENT") != 0)
                {
                    i++;
                    free(originalString);
                    memset(stringCheck, 0, strlen(stringCheck));
                    originalString = calloc(strlen(storedText[i])+1, sizeof(char));
                    strcpy(originalString, storedText[i]);
                    stringCheck = strtok(storedText[i], ";:");
//printf("up %s\n", stringCheck);
                    if (stringCheck == NULL || storedText[i] == '\0')
                    {
                        obj = NULL;
                        //free(*obj); do a clear calendar maybe
                        free(originalString);
                        return INV_EVENT;
                    }

                    if (strcmp(stringCheck, "END") == 0)
                    {
                        while (t != strlen(originalString))
                        {
                            originalString[t] = toupper(originalString[t]);
                            t++;
                        } t=0;
                        if (strcmp(originalString, "END:VCALENDAR") == 0)
                        {
                            obj = NULL;
                            free(originalString);
                            return INV_EVENT;
                        }
                        else if (strcmp(originalString, "END:VEVENT") == 0)
                        {
                            continue;
                        }
                    }

                    while (t != strlen(stringCheck))
                    {
                        stringCheck[t] = toupper(stringCheck[t]);
                        t++;
                    } t = 0;

                    if (strcmp(stringCheck, "UID") == 0)
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
                        if (uidCount == 1)
                        {
                            obj = NULL;
                            return INV_EVENT;
                        }
                        uidCount = 1;
                        stringCheck = strtok(NULL, ";:");
                        strcpy(event->UID, stringCheck);
                        memset(stringCheck, 0, strlen(stringCheck));
                        continue;
                    }
                    else if (strcmp(stringCheck, "DTSTAMP") == 0)
                    {
                        if (dtCount == 1)
                        {
                            obj = NULL;
                            return INV_CREATEDT;
                        }
                        toFree = returnDT(originalString);
                        if (toFree == NULL)
                        {
                            obj = NULL;
                            return INV_CREATEDT;
                        }
                        dtCount = 1;
                        char * getDT = strtok(toFree, "#");
                        strcpy(event->creationDateTime.date, getDT);
                        getDT = strtok(NULL, "#");
                        strcpy(event->creationDateTime.time, getDT);
                        getDT = strtok(NULL, "#");
//(*obj)->event = event;

                        if (strcmp(getDT, "1") == 0)
                        {
                            event->creationDateTime.UTC = 1;
                        }
                        else
                        {
                            event->creationDateTime.UTC = 0;
                        }
                        free(toFree);
                        memset(stringCheck, 0, strlen(stringCheck));
                        continue;
                    }

                    while (t != strlen(originalString))
                    {
                        originalString[t] = toupper(originalString[t]);
                        t++;
                    } t = 0;

                    if (strcmp(originalString, "BEGIN:VALARM") == 0) //alarm TING
                    {
//printf("hello\n");
                            Alarm * alarm = malloc(sizeof(Alarm));
                            //event->alarms = initializeList(&printFunc, &deleteFunc, &compareFunc); //maybe put higher?
                            alarm->properties = initializeList(&printFunc, &deleteFunc, &compareFunc); // maybe put higher??
                            List property = initializeList(&printFunc, &deleteFunc, &compareFunc);
                            alarm->properties = property;
                            alarm->trigger = malloc(sizeof(char));
//printf("hi\n");
                            //Alarm * alarm = malloc(sizeof(Alarm));

                            while (strcmp(originalString, "END:VALARM") != 0)
                            {
//printf("why hello\n");
                                i++;
                                memset(stringCheck, 0, strlen(stringCheck));
                                originalString = calloc(strlen(storedText[i])+1, sizeof(char));
                                if (originalString == NULL) // need to do this for most errors, and also need to free some other stuff before quitting
                                {
                                    obj = NULL;
                                    return OTHER_ERROR;
                                }
                                strcpy(originalString, storedText[i]);
                                stringCheck = strtok(storedText[i], ";:");

                                if (stringCheck == NULL || originalString[strlen(originalString)-1] == ';' || originalString[strlen(originalString)-1] == ':')
                                {
                                    obj = NULL;
                                    return INV_EVENT;
                                }


                                while (t != strlen(stringCheck))
                                {
                                    stringCheck[t] = toupper(stringCheck[t]);
                                    t++;
                                } t = 0;
//printf("why super hello\n");
                                if (strcmp(stringCheck, "TRIGGER") == 0)
                                {
                                      if (originalString[strlen(originalString)-1] == ';' || originalString[strlen(originalString)-1] == ':' || triggerCount == 1)
                                      {
                                          obj = NULL;
                                          free(originalString);
                                          return INV_EVENT;
                                      }

                                      char * stringSplit = malloc(sizeof(char) * strlen(originalString));
                                      if (stringSplit == NULL)
                                      {
                                          obj = NULL;
                                          free(originalString);
                                          return OTHER_ERROR;
                                      }
//printf("coincidence\n");
                                      while (t != strlen(originalString))
                                      {
                                          if (originalString[t-1] == ':' || originalString[t-1] == ';' || z > 0)
                                          {
                                              stringSplit[z] = originalString[t];
                                              z++;
                                          }
                                          t++;
                                      } t = 0;
                                      //Alarm * alarm = malloc(sizeof(Alarm));
//printf("prob good still\n");
                                      strcpy(alarm->trigger, stringSplit);
//printf("bloop\n");
                                      insertBack(&(**obj).event->alarms, alarm);
                                      triggerCount = 1;
//printf("blop\n");
                                }
                                else if (strcmp(stringCheck, "ACTION") == 0)
                                {
//printf("idek\n");
                                      if (originalString[strlen(originalString)-1] == ';' || originalString[strlen(originalString)-1] == ':' || actionCount == 1)
                                      {
                                          obj = NULL;
                                          free(originalString);
                                          return INV_EVENT;
                                      }

                                      char * stringSplit = malloc(sizeof(char) * strlen(originalString));
                                      if (stringSplit == NULL)
                                      {
                                          obj = NULL;
                                          free(originalString);
                                          return OTHER_ERROR;
                                      }

                                      while (t != strlen(originalString))
                                      {
                                          if (originalString[t-1] == ':' || originalString[t-1] == ';' || z > 0)
                                          {
                                              stringSplit[z] = originalString[t];
                                              z++;
                                          }
                                          t++;
                                      } t = 0;
                                      z = 0;
                                      //Alarm * alarm = malloc(sizeof(Alarm));
                                      strcpy(alarm->trigger, stringSplit);
                                      insertBack(&(**obj).event->alarms, alarm);
                                      actionCount = 1;
                                }
                                else // optional properties
                                {
                                      if (originalString[strlen(originalString)-1] == ';' || originalString[strlen(originalString)-1] == ':')
                                      {
                                          obj = NULL;
                                          free(originalString);
                                          return INV_EVENT;
                                      }

                                      char * stringSplit = malloc(sizeof(char) * strlen(originalString));
                                      if (stringSplit == NULL)
                                      {
                                          obj = NULL;
                                          free(originalString);
                                          return OTHER_ERROR;
                                      }

                                      while (t != strlen(originalString))
                                      {
                                          if (originalString[t-1] == ':' || originalString[t-1] == ';' || z > 0)
                                          {
                                              stringSplit[z] = originalString[t];
                                              z++;
                                          }
                                          t++;
                                      } t = 0;
                                      z = 0;
                                      //Alarm * alarm = malloc(sizeof(Alarm));
                                      strcpy(alarm->trigger, stringSplit);
                                      insertBack(&(**obj).event->alarms, alarm);

                                }
//printf("why damn\n");
                                if (strcmp(originalString, "END:VCALENDAR") == 0)
                                {
                                    obj = NULL;
                                    free(originalString);
                                    return INV_EVENT;
                                }
                                free(originalString);
                                memset(stringCheck, 0, strlen(stringCheck));
                            }
                            if (!(triggerCount == 1 || actionCount == 1))
                            {
                                obj = NULL;
                                return INV_EVENT;
                            }
                            triggerCount = 0;
                            actionCount = 0;
                    }
                    else
                    {
//printf("cancer\n");
//                        event->properties = initializeList(printFunc, deleteFunc, compareFunc);
//printf("%c\n", originalString[strlen(originalString)-1]);
                        if (originalString[strlen(originalString)-1] == ';' || originalString[strlen(originalString)-1] == ':')
                        {
                            obj = NULL;
                            free(originalString);
                            return INV_EVENT;
                        }
                        Property * property = malloc(sizeof(Property) + strlen(originalString) * sizeof(char));
                        strcpy(property->propName, stringCheck);
                        memset(stringCheck, 0, strlen(stringCheck));
                        char * stringSplit = malloc(sizeof(char) * strlen(originalString));
                        if (stringSplit == NULL)
                        {
                            obj = NULL;
                            free(originalString);
                            return OTHER_ERROR;
                        }

                        while (t != strlen(originalString))
                        {
                            if (originalString[t-1] == ':' || originalString[t-1] == ';' || z > 0)
                            {
                                stringSplit[z] = originalString[t];
                                z++;
                            }
                            t++;
                        } t = 0;
                        z = 0;
                        strcpy(property->propDescr, stringSplit);
                        insertBack(&(**obj).event->properties, property);
                        free(stringSplit);
                    }
                }
                //(*obj)->event = event;
                //free(originalString);
                //continue;
                //i--;
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

    if (idCount == 0 || versionCount == 0 || eventCount == 0) //and other required properties missing...etc.
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
    if (obj != NULL)
    { // think we'll need clear list function here as well
        //free(obj->event.creationDateTime);
	//free(obj->event->alarms.alarm->trigger);
        free(obj->event);
        free(obj);
    }
    return;
}

char* printCalendar(const Calendar* obj)
{

    return NULL;
}

char* printError(ErrorCode err)
{
    if (err == OK)
    {
        return "OK";
    }
    else if (err == INV_FILE)
    {
        return "INV_FILE";
    }
    else if (err == INV_CAL)
    {
        return "INV_CAL";
    }
    else if (err == INV_VER)
    {
        return "INV_VER";
    }
    else if (err == DUP_VER)
    {
        return "DUP_VER";
    }
    else if (err == INV_PRODID)
    {
        return "INV_PRODID";
    }
    else if (err == DUP_PRODID)
    {
        return "DUP_PRODID";
    }
    else if (err == INV_EVENT)
    {
        return "INV_EVENT";
    }
    else if (err == INV_CREATEDT)
    {
        return "INV_CREATEDT";
    }
    return "OTHER_ERROR";
}

