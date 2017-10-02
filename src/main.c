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
    printf("%s\n", printCalendar(calendar));

/*
printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf("%lf / version\n", calendar->version);
    printf("%s / product ID\n", calendar->prodID);
    printf("%s / UID\n", calendar->event->UID);
     printf("%s / date\n", calendar->event->creationDateTime.date);
 printf("%s / time\n", calendar->event->creationDateTime.time);
 printf("%d / UTC\n", calendar->event->creationDateTime.UTC);


void* elem;
void* elem2;
void * elem3;
ListIterator iter = createIterator(calendar->event->properties);
while ((elem = nextElement(&iter)) != NULL){
		Property * property = (Property*)elem;
		

		char* str = property->propName;
		printf("balls %s\n", str);
                char * str2 = property->propDescr;
                printf("balls2 %s\n", str2);
	}

if (calendar->event->alarms.head == NULL)
    printf("wtf\n");
ListIterator iter2  = createIterator(calendar->event->alarms);
while ((elem2 = nextElement(&iter2)) != NULL){
		Alarm * alarm = (Alarm*)elem2;
		printf("alarm trigger wtf %s\n", alarm->trigger);
                printf("alarm action %s\n", alarm->action);
	}

*/

    //double test = (*calendar)->version;
//printf("hello\n");
   // free(*calendar);
   // free(calendar);
  //  printf("wat\n %lf\n", test);
    //printf("%lf\n", (*calendar)->version);
    //printf("%s\n", (char*)error);
    //free(calendar); //free inside out prolly

    return 0;
}
