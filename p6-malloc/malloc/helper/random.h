#ifndef RANDOM_H_
#define RANDOM_H_

static unsigned int seed = 1;

void srandom (int newseed) {
    seed = (unsigned)newseed & 0x7fffffffU;
}

int random (void) {
    seed = (seed * 1103515245U + 12345U) & 0x7fffffffU;
    return (int)seed;
}

#endif
