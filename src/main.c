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
    createCalendar(argv[1], &calendar);
//printf("wat\n");
    printf("%lf test\n", calendar->version);
    printf("%s test\n", calendar->prodID);
    printf("%s test\n", calendar->event->UID);
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
