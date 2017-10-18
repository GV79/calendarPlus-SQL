#include "CalendarParser.h"

int main(int argc, char ** argv)
{
    Calendar * calendar = NULL;
    printf("%s\n", printError(createCalendar(argv[1], &calendar)));
    char * temp = printCalendar(calendar);
    printf("%s\n", temp);
    free(temp);
    deleteCalendar(calendar);
    return 0;
}
