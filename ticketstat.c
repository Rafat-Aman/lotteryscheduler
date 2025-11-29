#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        printf(1, "Usage: ticketstat pid\n");
        exit();
    }

    int pid = atoi(argv[1]);
    struct ticketinfo info;

    if (update_ticket_status(pid, &info) < 0) {
        printf(1, "Error: invalid PID\n");
        exit();
    }

    printf(1,
        "PID %d → base=%d, acc=%d, exch=%d, ticks=%d\n",
        pid,
        info.base_tickets,
        info.accumulated_tickets,
        info.exchanged_tickets,
        info.ticks
    );

    exit();
}
