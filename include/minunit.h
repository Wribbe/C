/* file: minunit.h */
#ifndef MINUNIT_DEFINED
#define MINUNIT_DEFINED

#include <stdlib.h>
#include <string.h>

extern int tests_run;

//#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_assert(message, test) do { if (!(test)) { \
                                            size_t length = strlen(message); \
                                            char * msg = malloc(sizeof(char) * (length+1)); \
                                            memcpy(msg, message, length); \
                                            msg[length] = '\0'; \
                                            return msg; \
                                        } \
                                    } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                               if (message) return message; } while (0)

#endif
