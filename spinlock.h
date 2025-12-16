// Prevent multiple inclusion.
#ifndef SPINLOCK_H
#define SPINLOCK_H

// Mutual exclusion lock.
struct spinlock {
  uint locked;       // Is the lock held?

  // For debugging:
  char *name;        // Name of lock.
  struct cpu *cpu;   // The cpu holding the lock.
  uint pcs[10];      // The call stack (an array of program counters)
  // that locked the lock.
};

struct ticketlock {
  uint next_ticket;
  uint now_serving;
  char *name;
};

#endif // SPINLOCK_H
