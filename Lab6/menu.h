#pragma once
#define AUTHOR_LEN 25
#define NAME_LEN 40
#define CATEGORY_LEN 25

struct Book
{
    char author[AUTHOR_LEN + 1];
    char name[NAME_LEN + 1];
    short publishingYear;
    unsigned int price;
    char category[CATEGORY_LEN + 1];
    char favorite;
};

void menu(struct Book * library, unsigned int librarySize, int * firstEmptyBook);