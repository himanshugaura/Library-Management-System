#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Book {
    char id[100];
    char title[100];
    char author[100];
    int issuedTo;
    struct Book* left;
    struct Book* right;
} Book;

Book* createBook(char id[], char title[], char author[]) {
    Book* newBook = (Book*)malloc(sizeof(Book));
    strcpy(newBook->id, id);
    strcpy(newBook->title, title);
    strcpy(newBook->author, author);
    newBook->issuedTo = 0;
    newBook->left = newBook->right = NULL;
    return newBook;
}

Book* insertBook(Book* root, char id[], char title[], char author[]) {
    if (root == NULL)
        return createBook(id, title, author);
    int cmp = strcmp(id, root->id);
    if (cmp < 0)
        root->left = insertBook(root->left, id, title, author);
    else if (cmp > 0)
        root->right = insertBook(root->right, id, title, author);
    else
        printf("Book with ID %s already exists!\n", id);
    return root;
}

Book* searchBook(Book* root, char id[]) {
    if (root == NULL || strcmp(root->id, id) == 0)
        return root;
    if (strcmp(id, root->id) < 0)
        return searchBook(root->left, id);
    return searchBook(root->right, id);
}

Book* minValueNode(Book* node) {
    Book* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

Book* deleteBook(Book* root, char id[]) {
    if (root == NULL)
        return root;
    int cmp = strcmp(id, root->id);
    if (cmp < 0)
        root->left = deleteBook(root->left, id);
    else if (cmp > 0)
        root->right = deleteBook(root->right, id);
    else {
        if (root->left == NULL) {
            Book* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Book* temp = root->left;
            free(root);
            return temp;
        }
        Book* temp = minValueNode(root->right);
        strcpy(root->id, temp->id);
        strcpy(root->title, temp->title);
        strcpy(root->author, temp->author);
        root->issuedTo = temp->issuedTo;
        root->right = deleteBook(root->right, temp->id);
    }
    return root;
}

void displayBooks(Book* root) {
    if (root != NULL) {
        displayBooks(root->left);
        printf("ID: %s, Title: %s, Author: %s, ", root->id, root->title, root->author);
        if (root->issuedTo)
            printf("Issued to Student ID: %d\n", root->issuedTo);
        else
            printf("Available\n");
        displayBooks(root->right);
    }
}

void issueBook(Book* root, char id[], int studentId) {
    Book* found = searchBook(root, id);
    if (found) {
        if (found->issuedTo == 0) {
            found->issuedTo = studentId;
            printf("Book ID %s issued to Student ID %d.\n", id, studentId);
        } else {
            printf("Book ID %s is already issued to Student ID %d.\n", id, found->issuedTo);
        }
    } else {
        printf("Book ID %s not found.\n", id);
    }
}

void returnBook(Book* root, char id[], int studentId) {
    Book* found = searchBook(root, id);
    if (found) {
        if (found->issuedTo == studentId) {
            found->issuedTo = 0;
            printf("Book ID %s returned by Student ID %d.\n", id, studentId);
        } else if (found->issuedTo == 0) {
            printf("Book ID %s is not issued to anyone.\n", id);
        } else {
            printf("Book ID %s is not issued to Student ID %d (issued to Student ID %d).\n", id, studentId, found->issuedTo);
        }
    } else {
        printf("Book ID %s not found.\n", id);
    }
}

void freeBST(Book* root) {
    if (root != NULL) {
        freeBST(root->left);
        freeBST(root->right);
        free(root);
    }
}

int main() {
    Book* root = NULL;
    int choice, studentId;
    char id[100], title[100], author[100], input[100];

    do {
        printf("\n--- Library Management System ---\n");
        printf("1. Add Book\n2. Search Book\n3. Display All Books\n4. Delete Book\n5. Issue Book\n6. Return Book\n7. Exit\n");

        printf("Enter your choice: ");
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
        case 1:
            printf("Enter Book ID: ");
            fgets(id, sizeof(id), stdin);
            id[strcspn(id, "\n")] = 0;
            printf("Enter Title: ");
            fgets(title, sizeof(title), stdin);
            title[strcspn(title, "\n")] = 0;
            printf("Enter Author: ");
            fgets(author, sizeof(author), stdin);
            author[strcspn(author, "\n")] = 0;
            root = insertBook(root, id, title, author);
            printf("Book added successfully.\n");
            break;
        case 2:
            printf("Enter Book ID to search: ");
            fgets(id, sizeof(id), stdin);
            id[strcspn(id, "\n")] = 0;
            {
                Book* found = searchBook(root, id);
                if (found) {
                    printf("Book found: ID: %s, Title: %s, Author: %s, ", found->id, found->title, found->author);
                    if (found->issuedTo)
                        printf("Issued to Student ID: %d\n", found->issuedTo);
                    else
                        printf("Available\n");
                } else {
                    printf("Book not found.\n");
                }
            }
            break;
        case 3:
            printf("Books in Library:\n");
            displayBooks(root);
            break;
        case 4:
            printf("Enter Book ID to delete: ");
            fgets(id, sizeof(id), stdin);
            id[strcspn(id, "\n")] = 0;
            root = deleteBook(root, id);
            printf("Book deleted (if existed).\n");
            break;
        case 5:
            printf("Enter Book ID to issue: ");
            fgets(id, sizeof(id), stdin);
            id[strcspn(id, "\n")] = 0;
            printf("Enter Student ID: ");
            fgets(input, sizeof(input), stdin);
            if (sscanf(input, "%d", &studentId) != 1) {
                printf("Invalid Student ID.\n");
                break;
            }
            issueBook(root, id, studentId);
            break;
        case 6:
            printf("Enter Book ID to return: ");
            fgets(id, sizeof(id), stdin);
            id[strcspn(id, "\n")] = 0;
            printf("Enter Student ID: ");
            fgets(input, sizeof(input), stdin);
            if (sscanf(input, "%d", &studentId) != 1) {
                printf("Invalid Student ID.\n");
                break;
            }
            returnBook(root, id, studentId);
            break;
        case 7:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice!\n");
        }
    } while (choice != 7);

    freeBST(root);
    return 0;
}