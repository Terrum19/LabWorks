#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "menu.h"

void flush_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void swap_books(struct book *a, struct book *b) {
    struct book temp = *a;
    *a = *b;
    *b = temp;
}

void printLib(struct book *library, int *librarySize, int *firstEmptyBook) {
    unsigned int bookCount = 0;
    *firstEmptyBook = *librarySize;

    for (int i = 0; i < *librarySize; i++) {
        if (library[i].inUse != 1) {
            if (i < *firstEmptyBook) {
                *firstEmptyBook = i;
            }
            continue;
        }
        bookCount++;
        printf("%d: %s, %s, %hd, %u, %s, %hd\n", i + 1,
               library[i].author, library[i].name, library[i].publishingYear,
               library[i].price, library[i].category, library[i].favorite);
    }
    printf("Current book amount: %u\n", bookCount);
}

void ensureNewBookAddAuthor(struct book *library, int *firstEmptyBook) {
    do {
        printf("Now enter the books author (<= 25 characters): ");
        if (fgets(library[*firstEmptyBook].author, 26, stdin) == NULL) {
            printf("Error reading input. Retry...\n");
            continue;
        }
        library[*firstEmptyBook].author[strcspn(library[*firstEmptyBook].author, "\n")] = 0;
        if (strlen(library[*firstEmptyBook].author) > 25) {
            printf("Too long! Max 25 chars. Retry...\n");
        } else {
            break;
        }
    } while (1);
}

void ensureNewBookAddName(struct book *library, int *firstEmptyBook) {
    do {
        printf("Now enter the books name (<= 40 characters): ");
        if (fgets(library[*firstEmptyBook].name, 41, stdin) == NULL) {
            printf("Error reading input. Retry...\n");
            continue;
        }
        library[*firstEmptyBook].name[strcspn(library[*firstEmptyBook].name, "\n")] = 0;
        if (strlen(library[*firstEmptyBook].name) > 40) {
            printf("Too long! Max 40 chars. Retry...\n");
        } else {
            break;
        }
    } while (1);
}

void ensureNewBookAddYear(struct book *library, int *firstEmptyBook) {
    short year;
    printf("Enter publishing year (-32768 to 32767): ");
    while (scanf("%hd", &year) != 1) {
        printf("Invalid input! Retry: ");
        flush_stdin();
    }
    // flush_stdin();
    library[*firstEmptyBook].publishingYear = year;
}

void ensureNewBookAddPrice(struct book *library, int *firstEmptyBook) {
    unsigned int price;
    printf("Enter price (0 to 4294967295): ");
    while (scanf("%u", &price) != 1) {
        printf("Invalid input! Retry: ");
        flush_stdin();
    }
    flush_stdin();
    library[*firstEmptyBook].price = price;
}

void ensureNewBookAddCategory(struct book *library, int *firstEmptyBook) {
    do {
        printf("Enter category (<= 25 characters): ");
        if (fgets(library[*firstEmptyBook].category, 26, stdin) == NULL) {
            printf("Error reading input. Retry...\n");
            continue;
        }
        library[*firstEmptyBook].category[strcspn(library[*firstEmptyBook].category, "\n")] = 0;
        if (strlen(library[*firstEmptyBook].category) > 25) {
            printf("Too long! Max 25 chars. Retry...\n");
        } else {
            break;
        }
    } while (1);
}

void ensureNewBookAddFavorite(struct book *library, int *firstEmptyBook) {
    short fav;
    printf("Is this your favorite? (1 = yes, 0 = no): ");
    while (scanf("%hd", &fav) != 1 || (fav != 0 && fav != 1)) {
        printf("Enter 0 or 1: ");
        flush_stdin();
    }
    flush_stdin();
    library[*firstEmptyBook].favorite = fav;
}

void addNewBook(struct book *library, int *librarySize, int *firstEmptyBook) {
    if (*firstEmptyBook >= *librarySize) {
        printf("Library is full!\n");
        return;
    }

    ensureNewBookAddAuthor(library, firstEmptyBook);
    ensureNewBookAddName(library, firstEmptyBook);
    ensureNewBookAddYear(library, firstEmptyBook);
    ensureNewBookAddPrice(library, firstEmptyBook);
    ensureNewBookAddCategory(library, firstEmptyBook);
    ensureNewBookAddFavorite(library, firstEmptyBook);

    library[*firstEmptyBook].inUse = 1;

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
            case 1: ensureNewBookAddAuthor(library, firstEmptyBook); break;
            case 2: ensureNewBookAddName(library, firstEmptyBook); break;
            case 3: ensureNewBookAddYear(library, firstEmptyBook); break;
            case 4: ensureNewBookAddPrice(library, firstEmptyBook); break;
            case 5: ensureNewBookAddCategory(library, firstEmptyBook); break;
            case 6: ensureNewBookAddFavorite(library, firstEmptyBook); break;
            case 7: editMode = 0; break;
            default: printf("Invalid option!\n");
        }
    }

    (*firstEmptyBook)++;
    printf("Book added successfully!\n");
}

void deleteBooks(struct book *library, int *librarySize, int *firstEmptyBook) {
    if (*firstEmptyBook == 0) {
        printf("Library is empty!\n");
        return;
    }

    printf("Enter book IDs to delete (one by one). Enter 0 to finish:\n");

    int id;
    int deleted = 0;
    while (1) {
        printf("ID (0 to stop): ");
        if (scanf("%d", &id) != 1) {
            flush_stdin();
            continue;
        }
        flush_stdin();

        if (id == 0) break;
        if (id < 1 || id > *librarySize) {
            printf("Invalid ID: %d (must be 1-%d)\n", id, *librarySize);
            continue;
        }
        if (library[id - 1].inUse != 1) {
            printf("Book %d already deleted or doesn't exist.\n", id);
            continue;
        }

        library[id - 1].inUse = 0;
        deleted++;
        printf("Book %d deleted.\n", id);
    }

    if (deleted == 0) {
        printf("No books were deleted.\n");
        return;
    }


    *firstEmptyBook = *librarySize;
    for (int i = 0; i < *librarySize; i++) {
        if (library[i].inUse != 1) {
            *firstEmptyBook = i;
            break;
        }
    }

    printf("%d book(s) deleted.\n", deleted);
}


void sortLibraryByAuthor(struct book *library, int *librarySize) {
    for (int i = 0; i < *librarySize - 1; i++) {
        for (int j = 0; j < *librarySize - i - 1; j++) {
            if (library[j].inUse == 1 && library[j + 1].inUse == 1) {
                if (strcmp(library[j].author, library[j + 1].author) > 0) {
                    swap_books(&library[j], &library[j + 1]);
                }
            }
        }
    }
    printf("Library sorted by author alphabetically.\n");
}

void sortLibraryByCategory(struct book *library, int *librarySize) {
    for (int i = 0; i < *librarySize - 1; i++) {
        for (int j = 0; j < *librarySize - i - 1; j++) {
            if (library[j].inUse == 1 && library[j + 1].inUse == 1) {
                if (strcmp(library[j].category, library[j + 1].category) > 0) {
                    swap_books(&library[j], &library[j + 1]);
                }
            }
        }
    }
    printf("Library sorted by category alphabetically.\n");
}

void sortLibraryByName(struct book *library, int *librarySize) {
    for (int i = 0; i < *librarySize - 1; i++) {
        for (int j = 0; j < *librarySize - i - 1; j++) {
            if (library[j].inUse == 1 && library[j + 1].inUse == 1) {
                if (strcmp(library[j].name, library[j + 1].name) > 0) {
                    swap_books(&library[j], &library[j + 1]);
                }
            }
        }
    }
    printf("Library sorted by name alphabetically.\n");
}

void sortLibraryByYear(struct book *library, int *librarySize) {
    for (int i = 0; i < *librarySize - 1; i++) {
        for (int j = 0; j < *librarySize - i - 1; j++) {
            if (library[j].inUse == 1 && library[j + 1].inUse == 1) {
                if (library[j].publishingYear > library[j + 1].publishingYear) {
                    swap_books(&library[j], &library[j + 1]);
                }
            }
        }
    }
    printf("Library sorted by publishing year.\n");
}

void sortLibraryByPrice(struct book *library, int *librarySize) {
    for (int i = 0; i < *librarySize - 1; i++) {
        for (int j = 0; j < *librarySize - i - 1; j++) {
            if (library[j].inUse == 1 && library[j + 1].inUse == 1) {
                if (library[j].price > library[j + 1].price) {
                    swap_books(&library[j], &library[j + 1]);
                }
            }
        }
    }
    printf("Library sorted by price.\n");
}

void sortLibraryByFavorite(struct book * library, int * librarySize){
    for (int i = 0; i < *librarySize - 1; i++) {
        for (int j = 0; j < *librarySize - i - 1; j++) {
            if (library[j].inUse == 1 && library[j + 1].inUse == 1) {
                if (library[j].favorite > library[j + 1].favorite) {
                    swap_books(&library[j], &library[j + 1]);
                }
            }
        }
    }
    printf("Library sorted by price.\n");
}

// void exportLibrary(struct book *library, int * librarySize){
//     FILE * fileStream;
//     fileStream = fopen("library.txt", "w");
    
//     for (int i = 0; i < (*librarySize); i++){
//         fprintf(fileStream, "%d: %s, %s, %hd, %u, %s, %hd\n", i + 1,
//                library[i].author, library[i].name, library[i].publishingYear,
//                library[i].price, library[i].category, library[i].favorite);
//     }
//     fclose(fileStream);
// }

void exportLibraryText(struct book *library, int *librarySize) {
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

    fprintf(file, "%d\n", *librarySize);

    for (int i = 0; i < *librarySize; i++) {
        if (library[i].inUse == 1) {
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

void importLibraryText(struct book *library, int *librarySize, int * firstEmptyBook) {
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

    if (savedSize > *librarySize) {
        printf("Error: import file book amount (%d), is bigger then the library size (%d).\n", savedSize, *librarySize);
        fclose(file);
        return;
    }

    for (int i = 0; i < *librarySize; i++) {
        library[i].inUse = 0;
    }

    int loaded = 0;
    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char author[26], name[41], category[26], favorite;
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
            library[pos].inUse = 1;

            loaded++;
        }
    }

    fclose(file);

    *firstEmptyBook = loaded;

    printf("Loaded %d books from file: %s\n", loaded, filename);
}

void librarySorting(struct book *library, int *librarySize) {
    printf("Choose sorting mode:\n");
    printf("a - By author (alphabetically)\n");
    printf("n - By book title (alphabetically)\n");
    printf("y - By publication year (ascending)\n");
    printf("p - By price (ascending)\n");
    printf("f - Favorites first (1), then others\n");
    printf("Your choice (a/n/y/p/f): ");

    char desiredOption;
    int valid = 0;

    while (!valid) {
        if (scanf(" %c", &desiredOption) != 1) {
            printf("Input error. Try again: ");
            flush_stdin();
            continue;
        }
        flush_stdin();

        switch (desiredOption) {
            case 'a':
            case 'A':
                sortLibraryByAuthor(library, librarySize);
                valid = 1;
                break;
            case 'n':
            case 'N':
                sortLibraryByName(library, librarySize);
                valid = 1;
                break;
            case 'y':
            case 'Y':
                sortLibraryByYear(library, librarySize);
                valid = 1;
                break;
            case 'p':
            case 'P':
                sortLibraryByPrice(library, librarySize);
                valid = 1;
                break;
            case 'f':
            case 'F':
                sortLibraryByFavorite(library, librarySize);
                valid = 1;
                break;
            default:
                printf("Invalid choice! Enter a, n, y, p or f: ");
                break;
        }
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

void menu(struct book *library, int *librarySize, int *firstEmptyBook) {
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
                librarySorting(library, librarySize);
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