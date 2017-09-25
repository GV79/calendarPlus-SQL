#include "CalendarParser.c"

int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        printf("Only one input file argument.\n");
        exit(0);
    }
    Calendar * calendar;
    createCalendar(argv[1], &calendar);
    return 0;
}
