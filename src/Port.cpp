#include "Port.h"

#define DEFINE_PORT(xx) Port Port ## xx = { &DDR ## xx, &PIN ## xx, &PORT ##xx };

#ifdef PORTB
DEFINE_PORT(B);
#endif

#ifdef PORTC
DEFINE_PORT(C);
#endif

#ifdef PORTD
DEFINE_PORT(D);
#endif
