/* file: minunit.h */
#ifndef MINUNIT_DEFINED
#define MINUNIT_DEFINED

extern int tests_run;

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                               if (message) return message; } while (0)
#endif
