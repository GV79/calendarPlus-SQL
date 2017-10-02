#include "CalendarParser.c"

int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        printf("Only one input file argument.\n");
        exit(0);
    }
    //Calendar ** calendar;
    //Calendar ** calendar = malloc(sizeof(Calendar*));;
    Calendar * calendar = NULL;
    //(*calendar) = malloc(sizeof(Calendar));
    //(*calendar)->version = malloc(sizeof(float));
    //ErrorCode error;
    printf("%s\n", printError(createCalendar(argv[1], &calendar)));

//printf("wat\n");
    printf("%lf / version\n", calendar->version);
    printf("%s / product ID\n", calendar->prodID);
    printf("%s / UID\n", calendar->event->UID);
     printf("%s / date\n", calendar->event->creationDateTime.date);
 printf("%s / time\n", calendar->event->creationDateTime.time);
 printf("%d / UTC\n", calendar->event->creationDateTime.UTC);


void* elem;
ListIterator iter = createIterator(calendar->event->properties);
while ((elem = nextElement(&iter)) != NULL){
		Property * property = (Property*)elem;
		
		/*
		We use the printData function that we created to return a string representation 
		of the data associated with the current node
		*/
		char* str = property->propName;
		printf("balls %s\n", str);
                char * str2 = property->propDescr;
                printf("balls2 %s\n", str2);
		
		//Since list.printData dynamically allocates the string, we must free it
		//free(str);
                //free(str2);
	}


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
