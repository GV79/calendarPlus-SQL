#include "LinkedListAPI.c"
#include "../include/CalendarParser.h"
#include <ctype.h>
//delete comments at the end


int compareFunc(const void *first, const void *second){
	
	return strcmp((char*)first, (char*)second);
}

void deleteProperty(void * toBeDeleted)
{
    if (toBeDeleted != NULL)
        free(toBeDeleted);
    return;
}

void deleteAlarm(void * toBeDeleted)
{
    if (toBeDeleted != NULL)
    {
        Alarm * temp;
        temp = (Alarm*)toBeDeleted;
        free(temp->trigger);
        free(temp);
    }
    return;
}

char * printAlarm(void * toBePrinted)
{
    if (toBePrinted != NULL)
    {
        char * toReturn;
        Alarm * temp = (Alarm *)toBePrinted;
        int length = strlen(temp->action) + strlen(temp->trigger);
        toReturn = (char *)malloc(sizeof(char) * length + 100);
        sprintf(toReturn, "Action: %s\nTrigger: %s\n", temp->action, temp->trigger);
        return toReturn;
    }
    return NULL;
}

char * printProperty(void * toBePrinted)
{
    if (toBePrinted != NULL)
    {
        char * toReturn;
        Property * temp = (Property *)toBePrinted;
        int length = strlen(temp->propName + strlen(temp->propDescr));
        toReturn = (char *)malloc(sizeof(char) * length + 100);
        sprintf(toReturn, "Name: %s\nDescription:%s\n", temp->propName, temp->propDescr);
        return toReturn;
    }
    return NULL;
}

char * returnVersion(char * text)
{
    char textVerify[7];
    float versionConverted = 0.0;
    int j, i;
    int length;
    j = 0;

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
    } i++;

    if (strcmp(textVerify, "VERSION") == 0 && (text[7] == ';' || text[7] == ':'))
    {
        length = strlen(text) - strlen(textVerify) - 1;
        char * version = malloc(sizeof(char) * length + 1);
        while (text[i] != '\n')
        {
             version[j] = text[i];
             i++;
             j++;
        }


        if (sscanf(version, "%f", &versionConverted) == 1)
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

    if (!(text[7] == ':' || text[7] == ';'))
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

    if (text[23] == 'Z')
    {
        timeVerify[0] = '1';
    }
    else
    {
        timeVerify[0] = '0';
    }

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

ErrorCode createCalendar(char* fileName, Calendar** obj)
{
    (*obj) = malloc(sizeof(Calendar));

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
    char * toFree = NULL;
    char * storedText[10000]; // free AFTER
    char * stringSplit;
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
            if (dataText[p] == ';' || dataText[p] == '\n')
            {
                continue;
            }

            lineCount++;
	    if (dataText[strlen(dataText)-1] == '\n')
            {
                    dataText[strlen(dataText)-1] = '\0';
            }
            if (lineCount == 1)
            {
                while (i != strlen(dataText))
                {
                    dataText[i] = toupper(dataText[i]);
                    i++;
                } i = 0;


             
	        if (strcmp(dataText, "BEGIN:VCALENDAR") != 0)
                {
                    obj = NULL;
                    return INV_CAL;
                }
            }
            else
            {
                storedText[lineCount - 2] = malloc(sizeof(char) * strlen(dataText) + 1);
                strcpy(storedText[lineCount - 2], dataText);
                memset(dataText, 0, strlen(dataText));
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
                    return INV_VER;
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

                if (strcmp(originalString, "BEGIN:VEVENT") == 0 && eventCount == 1)
                {
                    obj = NULL;
                    return INV_EVENT;
                }
                else if (strcmp(originalString, "BEGIN:VEVENT") != 0)
                {
                     free(originalString);
                     memset(stringCheck, 0, strlen(stringCheck));
           	     i++;
                     continue;
                }

                Event * event = malloc(sizeof(Event));
                (*obj)->event = event;
                event->properties = initializeList(&printProperty, &deleteProperty, &compareFunc);
                event->alarms = initializeList(&printAlarm, &deleteAlarm, &compareFunc);

                eventCount = 1;
                while (strcmp(originalString, "END:VEVENT") != 0)
                {
                    i++;
                    free(originalString);
                    memset(stringCheck, 0, strlen(stringCheck));
                    originalString = calloc(strlen(storedText[i])+1, sizeof(char));
                    strcpy(originalString, storedText[i]);
                    stringCheck = strtok(storedText[i], ";:");
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

                    while (t != strlen(originalString) && strcmp(stringCheck, "BEGIN") == 0)
                    {
                        originalString[t] = toupper(originalString[t]);
                        t++;
                    } t = 0;

                    if (strcmp(originalString, "BEGIN:VALARM") == 0)
                    {
                            Alarm * alarm = malloc(sizeof(Alarm));
                            List property = initializeList(&printProperty, &deleteProperty, &compareFunc);
                            alarm->properties = property;

                            while (strcmp(originalString, "END:VALARM") != 0)
                            {
                                i++;
                                memset(originalString, 0, strlen(originalString));
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

                                if (strcmp(stringCheck, "TRIGGER") == 0)
                                {
                                      if (originalString[strlen(originalString)-1] == ';' || originalString[strlen(originalString)-1] == ':' || triggerCount == 1)
                                      {
                                          obj = NULL;
                                          free(originalString);
                                          return INV_EVENT;
                                      }

                                      stringSplit = malloc(sizeof(char) * strlen(originalString));
                                      if (stringSplit == NULL)
                                      {
                                          obj = NULL;
                                          free(originalString);
                                          return OTHER_ERROR;
                                      }

                                      while (t != strlen(originalString))
                                      {
                                          t++;
                                          if (originalString[t-1] == ':' || originalString[t-1] == ';' || z > 0)
                                          {
                                              stringSplit[z] = originalString[t];
                                              z++;
                                          }
                                       
                                      } t = 0;

                                      alarm->trigger = malloc(sizeof(char) * strlen(stringSplit));
                                      strcpy(alarm->trigger, stringSplit);
                                      free(stringSplit);
                                      triggerCount = 1;
                                }
                                else if (strcmp(stringCheck, "ACTION") == 0)
                                {
                                      if (originalString[strlen(originalString)-1] == ';' || originalString[strlen(originalString)-1] == ':' || actionCount == 1)
                                      {
                                          obj = NULL;
                                          free(originalString);
                                          return INV_EVENT;
                                      }

                                      stringSplit = malloc(sizeof(char) * strlen(originalString));
                                      if (stringSplit == NULL)
                                      {
                                          obj = NULL;
                                          free(originalString);
                                          return OTHER_ERROR;
                                      }

                                      while (t != strlen(originalString))
                                      {
                                          t++;
                                          if (originalString[t-1] == ':' || originalString[t-1] == ';' || z > 0)
                                          {
                                              stringSplit[z] = originalString[t];
                                              z++;
                                          }
  
                                      } t = 0;
                                      z = 0;
                                      strcpy(alarm->action, stringSplit);
                                      free(stringSplit);
                                      actionCount = 1;
                                }
                                else if (strcmp(stringCheck, "END") == 0)
                                {
                                    while (t != strlen(originalString))
                                    {
                                        t++;
                                        originalString[t] = toupper(originalString[t]);
                                    } t = 0;
                                    if (strcmp(originalString, "END:VALARM") == 0)
                                    {
                                        continue;
                                    }
                                }
                                else // optional properties
                                {
                                      if (originalString[strlen(originalString)-1] == ';' || originalString[strlen(originalString)-1] == ':')
                                      {
                                          obj = NULL;
                                          free(originalString);
                                          return INV_EVENT;
                                      }

                                      stringSplit = malloc(sizeof(char) * strlen(originalString));
                                      if (stringSplit == NULL)
                                      {
                                          obj = NULL;
                                          free(originalString);
                                          return OTHER_ERROR;
                                      }
			              z = 0;
				      t = 0;
                                      while (t != strlen(originalString))
                                      {
                                          t++;
                                          if (originalString[t-1] == ':' || originalString[t-1] == ';' || z > 0)
                                          {
                                              stringSplit[z] = originalString[t];
                                              z++;
                                          }
                                         
                                      } t = 0;
                                      z = 0;

                                      Property * property = malloc(sizeof(Property) + strlen(originalString) * sizeof(char));
                                      strcpy(property->propName, stringCheck);
                                      strcpy(property->propDescr, stringSplit);
                                      insertBack(&alarm->properties, property);
                                      free(stringSplit);
                                }
                                if (strcmp(originalString, "END:VCALENDAR") == 0)
                                {
                                    obj = NULL;
                                    free(originalString);
                                    return INV_EVENT;
                                }
                                free(originalString);
                                memset(stringCheck, 0, strlen(stringCheck));
                                memset(stringSplit, 0, strlen(stringSplit));
                            }
                            insertBack(&(**obj).event->alarms, alarm);
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
			memset(stringSplit, 0, strlen(stringSplit));
                        if (stringSplit == NULL)
                        {
                            obj = NULL;
                            free(originalString);
                            return OTHER_ERROR;
                        }
                        z = 0;
                        t = 0;

                        while (t != strlen(originalString)+1)
                        {
                            t++;
                            if (originalString[t-1] == ':' || originalString[t-1] == ';' || z > 0)
                            {
                                stringSplit[z] = originalString[t];
                                z++;
                            }
                        } t = 0;
                        z = 0;

                        strcpy(property->propDescr, stringSplit);
                        insertBack(&(**obj).event->properties, property);
                        memset(originalString, 0, strlen(originalString));
                        free(stringSplit);
                    }
                }
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
    return OK;
}

void deleteCalendar(Calendar* obj)
{
    if (obj != NULL)
    { // think we'll need clear list function here as well
        clearList(&obj->event->alarms);
        //free(obj->event.creationDateTime);
	//free(obj->event->alarms.alarm->trigger);
        free(obj->event);
        free(obj);
    }
    return;
}

char* printCalendar(const Calendar* obj)
{
    ListIterator iter, iter2, iter3;
    if (obj == NULL)
    {
        return "Invalid list\n";
    }
    char * string = malloc(sizeof(char) * 1000);
    char * concactenate = malloc(sizeof(char) * 100000);
    sprintf(string, "%s", "Allocated object\n");
    strcat(concactenate, string);
    sprintf(string, "%s", "File parsed successfully\n");
    strcat(concactenate, string);
    sprintf(string, "Calendar: version = %lf, prodID = %s\n", obj->version, obj->prodID);
    strcat(concactenate, string);
    sprintf(string, "%s", "Event\n");
    strcat(concactenate, string);
    sprintf(string, "\tUID = %s\n", obj->event->UID);
    strcat(concactenate, string);
    sprintf(string, "\tcreationDateTime = %s:%s, UTC=%d\n", obj->event->creationDateTime.date, obj->event->creationDateTime.time, obj->event->creationDateTime.UTC);
    strcat(concactenate, string);
    if (obj->event->alarms.head != NULL)
    {
        iter = createIterator(obj->event->alarms);
        void * elem;
        sprintf(string, "%s", "\tAlarms:");
        strcat(concactenate, string);
	while ((elem = nextElement(&iter)) != NULL)
        {
		Alarm * alarm = (Alarm*)elem;
                sprintf(string, "\n\t\tAction: %s\n", alarm->action);
                strcat(concactenate, string);
                sprintf(string, "\t\tTrigger: %s\n", alarm->trigger);
                strcat(concactenate, string);
                sprintf(string, "%s", "\t\tProperties:\n");
                strcat(concactenate, string);
		if (alarm->properties.head != NULL)
                {
                    void * elem2;
                    iter2 = createIterator(alarm->properties);
                    while ((elem2 = nextElement(&iter2)) != NULL)
                    {
                        Property * propertyValues = (Property*)elem2;
                        sprintf(string, "\t\t- %s:%s\n", propertyValues->propName, propertyValues->propDescr);
                        strcat(concactenate, string);
                    }
                }
	}
    }
    sprintf(string, "%s", "\n\n\tOther properties:\n");
    strcat(concactenate, string);

    if (obj->event->properties.head != NULL)
    {
        void * elem3;
        iter3 = createIterator(obj->event->properties);

        while ((elem3 = nextElement(&iter3)) != NULL)
        {
            Property * propertyValues2 = (Property*)elem3;
            sprintf(string, "\t\t-%s:%s\n", propertyValues2->propName, propertyValues2->propDescr);
            strcat(concactenate, string);
        }
         
    }
    free(string);
    return concactenate;
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

