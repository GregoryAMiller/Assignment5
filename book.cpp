#include "book.h"

Book::Book(int id, string bookName, string auth, string cat)
    :bookID(id), title(bookName), author(auth), category(cat)  {}


string Book::getTitle() {
    return title; // complete
}

string Book::getAuthor() {
    return author; // complete
}

string Book::getCategory() {
    return category; // complete
}

int Book::getId() {
    return bookID; // complete
}
void Book::setPersonPtr(Person * ptr) {
    personPtr = ptr;
} // complete

Person * Book::getPersonPtr() {
    return personPtr; // complete
}
