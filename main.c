#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Book {
    int id;
    char title[100];
    char author[100];
    int issuedTo; 
    struct Book* left;
    struct Book* right;
} Book;

Book* createBook(int id, char title[], char author[]) {
    Book* newBook = (Book*)malloc(sizeof(Book));
    newBook->id = id;
    strcpy(newBook->title, title);
    strcpy(newBook->author, author);
    newBook->issuedTo = 0; 
    newBook->left = newBook->right = NULL;
    return newBook;
}

Book* insertBook(Book* root, int id, char title[], char author[]) {
    if (root == NULL)
        return createBook(id, title, author);
    if (id < root->id)
        root->left = insertBook(root->left, id, title, author);
    else if (id > root->id)
        root->right = insertBook(root->right, id, title, author);
    else
        printf("Book with ID %d already exists!\n", id);
    return root;
}


Book* searchBook(Book* root, int id) {
    if (root == NULL || root->id == id)
        return root;
    if (id < root->id)
        return searchBook(root->left, id);
    return searchBook(root->right, id);
}


Book* minValueNode(Book* node) {
    Book* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}


Book* deleteBook(Book* root, int id) {
    if (root == NULL)
        return root;
    if (id < root->id)
        root->left = deleteBook(root->left, id);
    else if (id > root->id)
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
        root->id = temp->id;
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
        printf("ID: %d, Title: %s, Author: %s, ", root->id, root->title, root->author);
        if (root->issuedTo)
            printf("Issued to Student ID: %d\n", root->issuedTo);
        else
            printf("Available\n");
        displayBooks(root->right);
    }
}

void issueBook(Book* root, int bookId, int studentId) {
    Book* found = searchBook(root, bookId);
    if (found) {
        if (found->issuedTo == 0) {
            found->issuedTo = studentId;
            printf("Book ID %d issued to Student ID %d.\n", bookId, studentId);
        } else {
            printf("Book ID %d is already issued to Student ID %d.\n", bookId, found->issuedTo);
        }
    } else {
        printf("Book ID %d not found.\n", bookId);
    }
}

void returnBook(Book* root, int bookId, int studentId) {
    Book* found = searchBook(root, bookId);
    if (found) {
        if (found->issuedTo == studentId) {
            found->issuedTo = 0;
            printf("Book ID %d returned by Student ID %d.\n", bookId, studentId);
        } else if (found->issuedTo == 0) {
            printf("Book ID %d is not issued to anyone.\n", bookId);
        } else {
            printf("Book ID %d is not issued to Student ID %d (issued to Student ID %d).\n", bookId, studentId, found->issuedTo);
        }
    } else {
        printf("Book ID %d not found.\n", bookId);
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
    int choice, id, studentId;
    char title[100], author[100];

    do {
        printf("\n--- Library Management System ---\n");
        printf("1. Add Book\n2. Search Book\n3. Display All Books\n4. Delete Book\n5. Issue Book\n6. Return Book\n7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Enter Book ID: ");
            scanf("%d", &id);
            printf("Enter Title: ");
            while(getchar() != '\n');
            fgets(title, 100, stdin);
            title[strcspn(title, "\n")] = 0; 
            printf("Enter Author: ");
            fgets(author, 100, stdin);
            author[strcspn(author, "\n")] = 0; 
            root = insertBook(root, id, title, author);
            printf("Book added successfully.\n");
            break;
        case 2:
            printf("Enter Book ID to search: ");
            scanf("%d", &id);
            {
                Book* found = searchBook(root, id);
                if (found) {
                    printf("Book found: ID: %d, Title: %s, Author: %s, ", found->id, found->title, found->author);
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
            scanf("%d", &id);
            root = deleteBook(root, id);
            printf("Book deleted (if existed).\n");
            break;
        case 5:
            printf("Enter Book ID to issue: ");
            scanf("%d", &id);
            printf("Enter Student ID: ");
            scanf("%d", &studentId);
            issueBook(root, id, studentId);
            break;
        case 6:
            printf("Enter Book ID to return: ");
            scanf("%d", &id);
            printf("Enter Student ID: ");
            scanf("%d", &studentId);
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