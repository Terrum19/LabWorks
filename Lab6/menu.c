#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "menu.h"

void flush_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void swap_books(struct Book *a, struct Book *b) {
    struct Book temp = *a;
    *a = *b;
    *b = temp;
}

void printLib(struct Book *library, unsigned int librarySize, int *firstEmptyBook) {
    unsigned int bookCount = 0;
    *firstEmptyBook = librarySize;

    for (int i = 0; i < librarySize; i++) {
        if (strlen(library[i].name) == 0) {
            *firstEmptyBook = i;
            break;
        }
        // You said that it is not logical for the print function to change firstEmptyBook argument. But since most of the functions
        // somehow use this parameter updating it frequently seems to be preferable
        bookCount++;
        printf("%d: %s, %s, %hd, %u, %s, %hd\n", i + 1,
               library[i].author, library[i].name, library[i].publishingYear,
               library[i].price, library[i].category, library[i].favorite);
    }
    printf("Current book amount: %u\n", bookCount);
}

void ensureNewBookAddAuthor(struct Book *library, int firstEmptyBook) {
    do {
        printf("Now enter the books author (<= %d characters): ", AUTHOR_LEN);
        if (fgets(library[firstEmptyBook].author, AUTHOR_LEN, stdin) == NULL) {
            printf("Error reading input. Retry...\n");
            continue;
        }
        library[firstEmptyBook].author[strcspn(library[firstEmptyBook].author, "\n")] = 0;
        if (strlen(library[firstEmptyBook].author) > AUTHOR_LEN) {
            printf("Too long! Max %d chars. Retry...\n", AUTHOR_LEN);
        } else {
            break;
        }
    } while (1);
}

void ensureNewBookAddName(struct Book *library, int firstEmptyBook) {
    do {
        printf("Now enter the books name (<= %d characters): ", NAME_LEN);
        if (fgets(library[firstEmptyBook].name, NAME_LEN, stdin) == NULL) {
            printf("Error reading input. Retry...\n");
            continue;
        }
        library[firstEmptyBook].name[strcspn(library[firstEmptyBook].name, "\n")] = 0;
        if (strlen(library[firstEmptyBook].name) > NAME_LEN) {
            printf("Too long! Max %d chars. Retry...\n", NAME_LEN);
        } else {
            break;
        }
    } while (1);
}

void ensureNewBookAddYear(struct Book *library, int firstEmptyBook) {
    short year;
    printf("Enter publishing year (-32768 to 32767): ");
    while (scanf("%hd", &year) != 1) {
        printf("Invalid input! Retry: ");
        flush_stdin();
    }
    // flush_stdin();
    library[firstEmptyBook].publishingYear = year;
}

void ensureNewBookAddPrice(struct Book *library, int firstEmptyBook) {
    unsigned int price;
    printf("Enter price (0 to 4294967295): ");
    while (scanf("%u", &price) != 1) {
        printf("Invalid input! Retry: ");
        flush_stdin();
    }
    flush_stdin();
    library[firstEmptyBook].price = price;
}

void ensureNewBookAddCategory(struct Book *library, int firstEmptyBook) {
    do {
        printf("Enter category (<= %d characters): ", CATEGORY_LEN);
        if (fgets(library[firstEmptyBook].category, CATEGORY_LEN, stdin) == NULL) {
            printf("Error reading input. Retry...\n");
            continue;
        }
        library[firstEmptyBook].category[strcspn(library[firstEmptyBook].category, "\n")] = 0;
        if (strlen(library[firstEmptyBook].category) > CATEGORY_LEN) {
            printf("Too long! Max %d chars. Retry...\n", CATEGORY_LEN);
        } else {
            break;
        }
    } while (1);
}

void ensureNewBookAddFavorite(struct Book *library, int firstEmptyBook) {
    short fav;
    printf("Is this your favorite? (1 = yes, 0 = no): ");
    while (scanf("%hd", &fav) != 1 || (fav != 0 && fav != 1)) {
        printf("Enter 0 or 1: ");
        flush_stdin();
    }
    flush_stdin();
    library[firstEmptyBook].favorite = fav;
}

void addNewBook(struct Book *library, unsigned int librarySize, int *firstEmptyBook) {
    if (*firstEmptyBook >= librarySize) {
        printf("Library is full!\n");
        return;
    }

    ensureNewBookAddAuthor(library, *firstEmptyBook);
    ensureNewBookAddName(library, *firstEmptyBook);
    ensureNewBookAddYear(library, *firstEmptyBook);
    ensureNewBookAddPrice(library, *firstEmptyBook);
    ensureNewBookAddCategory(library, *firstEmptyBook);
    ensureNewBookAddFavorite(library, *firstEmptyBook);

    int editMode;
    printf("Do you want to edit the entered info? (1 = yes, 0 = no): ");
    scanf("%d", &editMode);
    flush_stdin();

    while (editMode == 1) {
        int option;
        printf("\nEDITING:\n");
        printf("1 - Author\n2 - Name\n3 - Year\n4 - Price\n5 - Category\n6 - Favorite\n7 - Exit editing\n");
        printf("Choose: ");
        scanf("%d", &option);
        flush_stdin();

        switch (option) {
            case 1: ensureNewBookAddAuthor(library, *firstEmptyBook); break;
            case 2: ensureNewBookAddName(library, *firstEmptyBook); break;
            case 3: ensureNewBookAddYear(library, *firstEmptyBook); break;
            case 4: ensureNewBookAddPrice(library, *firstEmptyBook); break;
            case 5: ensureNewBookAddCategory(library, *firstEmptyBook); break;
            case 6: ensureNewBookAddFavorite(library, *firstEmptyBook); break;
            case 7: editMode = 0; break;
            default: printf("Invalid option!\n");
        }
    }

    (*firstEmptyBook)++;
    printf("Book added successfully!\n");
}

int cmpByAuthor(const void *a, const void *b) {
    const struct Book *bookA = (const struct Book *)a;
    const struct Book *bookB = (const struct Book *)b;
    return strcmp(bookA->author, bookB->author);
}

int cmpByName(const void *a, const void *b) {
    const struct Book *bookA = (const struct Book *)a;
    const struct Book *bookB = (const struct Book *)b;
    return strcmp(bookA->name, bookB->name);
}

int cmpByYear(const void *a, const void *b) {
    const struct Book *bookA = (const struct Book *)a;
    const struct Book *bookB = (const struct Book *)b;
    return (int)(bookA->publishingYear - bookB->publishingYear);
}


int cmpByPrice(const void *a, const void *b) {
    const struct Book *bookA = (const struct Book *)a;
    const struct Book *bookB = (const struct Book *)b;
    if (bookA->price < bookB->price) return -1;
    if (bookA->price > bookB->price) return 1;
    return 0;
}

int cmpByCategory(const void *a, const void *b) {
    const struct Book *bookA = (const struct Book *)a;
    const struct Book *bookB = (const struct Book *)b;
    return strcmp(bookA->category, bookB->category);
}

int cmpByFavorite(const void *a, const void *b) {
    const struct Book *bookA = (const struct Book *)a;
    const struct Book *bookB = (const struct Book *)b;
    return (int)(bookB->favorite - bookA->favorite);
}

int cmpInt(const int *a, const int *b){
    if (*a < *b) return -1;
    else if (*a == *b) return 0;
    else return 1;
}

void deleteBooks(struct Book *library, unsigned int librarySize, int *numBooks) {
    if (*numBooks == 0) {
        printf("Library is empty!\n");
        return;
    }

    printf("Enter book IDs to delete (one by one). Enter 0 to finish:\n");

    int * idsToDelete = malloc(librarySize * sizeof(int));
    int deleteCount = 0;
    int id;
    while (1) {
        printf("ID (0 to stop): ");
        if (scanf("%d", &id) != 1) {
            flush_stdin();
            continue;
        }
        flush_stdin();

        if (id == 0) break;
        if (id < 1 || id > *numBooks) {
            printf("Invalid ID: %d (must be 1-%d)\n", id, *numBooks);
            continue;
        }
        idsToDelete[deleteCount++] = id - 1;
    }

    if (deleteCount == 0) {
        printf("No books were deleted.\n");
        return;
    }

    qsort(idsToDelete, deleteCount, sizeof(int), cmpInt);

    int writeIndex = 0;
    int deletionInd = 0;
    for (int read_idx = 0; read_idx < *numBooks; read_idx++) {
        if (deletionInd < deleteCount && read_idx == idsToDelete[deletionInd]) {
            library[idsToDelete[deletionInd]].name[0] = '\0';
            deletionInd++;  
            continue;
        }
        if (read_idx != writeIndex) {
            library[writeIndex] = library[read_idx];
        }
        writeIndex++;
    }

    for (int i = writeIndex; i < librarySize; i++) {
        library[i].name[0] = '\0';
    }

    *numBooks = writeIndex;
    printf("%d book(s) deleted.\n", deleteCount);
    free(idsToDelete);
}

void exportLibraryText(struct Book *library, unsigned int librarySize) {
    char filename[256];
    printf("Enter the export filename (e.g.: library.txt):\n> ");
    // flush_stdin();
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("err0r in reading the filename.\n");
        return;
    }
    filename[strcspn(filename, "\n")] = 0;

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: cant open the file: %s\n", filename);
        return;
    }

    fprintf(file, "%d\n", librarySize);

    for (int i = 0; i < librarySize; i++) {
        if (strlen(library[i].name) != 0) {
            fprintf(file, "%s|%s|%hd|%u|%s|%hd\n",
                    library[i].author,
                    library[i].name,
                    library[i].publishingYear,
                    library[i].price,
                    library[i].category,
                    library[i].favorite);
        }
    }

    fclose(file);
    printf("Saved successfully to: %s\n", filename);
}

void importLibraryText(struct Book *library, unsigned int librarySize, int * firstEmptyBook) {
    char filename[256];
    printf("Enter the filename for importing (e.g.: library.txt):\n> ");
    // flush_stdin();
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error in getting the filename\n");
        return;
    }
    filename[strcspn(filename, "\n")] = 0;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error in opening the file: %s\n", filename);
        return;
    }

    int savedSize;
    if (fscanf(file, "%d\n", &savedSize) != 1) {
        printf("Error in reading the library size\n");
        fclose(file);
        return;
    }

    if (savedSize > librarySize) {
        printf("Error: import file book amount (%d), is bigger then the library size (%d).\n", savedSize, librarySize);
        fclose(file);
        return;
    }


    int loaded = 0;
    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char author[AUTHOR_LEN + 1], name[NAME_LEN + 1], category[CATEGORY_LEN + 1], favorite;
        short year;
        unsigned int price;


        if (sscanf(line, "%25[^|]|%40[^|]|%hd|%u|%25[^|]|%hd",
                   author, name, &year, &price, category, &favorite) == 6) {

            int pos = loaded;
            strcpy(library[pos].author, author);
            strcpy(library[pos].name, name);
            library[pos].publishingYear = year;
            library[pos].price = price;
            strcpy(library[pos].category, category);
            library[pos].favorite = favorite;

            loaded++;
        }
    }

    fclose(file);

    *firstEmptyBook = loaded;

    printf("Loaded %d books from file: %s\n", loaded, filename);
}

void librarySorting(struct Book *library, int firstEmptyBook, unsigned int librarySize) {
    printf("Choose sorting mode:\n");
    printf("1 - By author (alphabetically)\n");
    printf("2 - By book title (alphabetically)\n");
    printf("3 - By publication year (ascending)\n");
    printf("4 - By price (ascending)\n");
    printf("5 - By category");
    printf("6 - Favorites first (1), then others\n");
    printf("Your choice (1, 2, 3, 4, 5, 6): ");

    short desiredOption;
    int valid = 0;
    typedef int (*comparator)(const void *, const void *);
    comparator comparatorsArr[] = {cmpByAuthor, cmpByName, cmpByYear, cmpByPrice, cmpByCategory, cmpByFavorite};

    while (!valid) {
        if (scanf("%hd", &desiredOption) != 1) {
            printf("Input error. Try again: ");
            flush_stdin();
            continue;
        }
        if (desiredOption > 6 || desiredOption < 1){
            printf("Thy number should be in range of 1 to 6. Try again: ");
            flush_stdin();
            continue;
        }
        flush_stdin();
        desiredOption -= 1;
        qsort(library, firstEmptyBook, sizeof(struct Book), comparatorsArr[desiredOption]);
        valid = 1;
    }

    printf("Sorting completed.\n");
}


void printOptions() {
    printf("\nWhat would you like to do?\n");
    printf("1 - Print library\n");
    printf("2 - Add new book\n");
    printf("3 - Delete books (enter IDs, 0 to finish)\n");
    printf("4 - Export current library into file\n");
    printf("5 - Import library from a file\n");
    printf("6 - Sort library\n");
    printf("0 - Exit\n");
    printf("Choice: ");
}

void menu(struct Book *library, unsigned int librarySize, int *firstEmptyBook) {
    short option;

    while (1) {
        printOptions();
        if (scanf("%hd", &option) != 1) {
            printf("Invalid input!\n");
            flush_stdin();
            continue;
        }
        flush_stdin();

        switch (option) {
            case 1:
                printLib(library, librarySize, firstEmptyBook);
                break;
            case 2:
                addNewBook(library, librarySize, firstEmptyBook);
                break;
            case 3:
                deleteBooks(library, librarySize, firstEmptyBook);
                break;
            case 4:
                exportLibraryText(library, librarySize);
                break;
            case 5:
                importLibraryText(library, librarySize, firstEmptyBook);
                break;
            case 6:
                librarySorting(library, *firstEmptyBook, librarySize);
                break;
            case 0:
                printf("Goodbye!\n");
                return;
            default:
                printf("Unknown option!\n");
                break;
        }
    }
}