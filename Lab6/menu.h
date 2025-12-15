struct book
{
    char author[26];
    char name[41];
    short publishingYear;
    unsigned int price;
    char category[26];
    char favorite;
    char inUse;
};

void menu(struct book * library, int * librarySize, int * firstEmptyBook);