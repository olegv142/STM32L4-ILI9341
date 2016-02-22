#pragma once 

namespace mbed {

void mbed_set_unbuffered_stream(FILE *_file);
int mbed_getc(FILE *_file);
char* mbed_gets(char *s, int size, FILE *_file);

class Stream {
public:
    Stream(const char *name=NULL) {}
    int putc(int c) { return 0; }
};

};

