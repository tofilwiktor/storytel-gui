#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <string>
#include <cstdint>

struct Book{
    std::string title;
    std::string author;
};

struct BookMark {
    uint64_t id;
    int64_t position;
};

struct ABook {
    uint64_t id;
};

struct BookEntry {
    ABook abook;
    BookMark bookmark;
    Book book;
};


#endif // STRUCTURES_H
