// Gregory Miller
// Section 2
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "person.cpp"
#include "book.cpp"

using namespace std;

void printMenu() {
    cout << "----------Library Book Rental System----------" << endl;
    cout << "1.  Book checkout" << endl;
    cout << "2.  Book return" << endl;
    cout << "3.  View all available books" << endl;
    cout << "4.  View all outstanding rentals" << endl;
    cout << "5.  View outstanding rentals for a cardholder" << endl;
    cout << "6.  Open new library card" << endl;
    cout << "7.  Close library card" << endl;
    cout << "8.  Exit system" << endl;
    cout << "Please enter a choice: ";
}

void readBooks(vector<Book *> &books) {
    ifstream inData;
    inData.open("books.txt");
    string bookId;
    string title;
    string author;
    string cat;
    string trash;
    while(inData >> bookId) {
      getline(inData, trash);
      getline(inData, title);
      getline(inData, author);
      getline(inData, cat);
      getline(inData, trash);
      int id = stoi(bookId);
      Book *bookPtr = new Book(id, title, author, cat);
      books.push_back(bookPtr);
    }
    inData.close();
}

int readPersons(vector<Person *> &cardholders) {
  ifstream inData;
  inData.open("persons.txt");
  int cardN;
  bool act;
  string fName;
  string lName;
  int i = 0;
  while(inData >> cardN >> act >> fName >> lName) {
    Person *personPtr = new Person(cardN, act, fName, lName);
    cardholders.push_back(personPtr);
    i++;
  }
  i--;
    inData.close();
    return cardholders.at(i)->getId() + 1;
}

void readRentals(vector<Book *> &books, vector<Person *> &cardholders) {
    ifstream inData;
    inData.open("rentals.txt");
    int bookId;
    int cardId;
    Person *personPtr = cardholders.at(0);
    while(inData >> bookId >> cardId) {
      // get personPtr in book object
      // get cardID
      for(int i = 0; i < cardholders.size(); i++) {
         if(cardId == cardholders.at(i)->getId()) {personPtr = cardholders.at(i);}
      }
      // i want to set the right books personPtr
     for(int i = 0; i < books.size(); i++) {
        if(bookId == books.at(i)->getId()) {books.at(i)->setPersonPtr(personPtr);}
      }
    }
    personPtr = nullptr;
    inData.close();
}
// void checkCardId(vector<Person *> cardholders) {
//
//
// }
void bookCheckout(vector<Book *> &books, vector<Person *> &cardholders) {
    int cardholderId;
    int cardIndex;
    int bookIndex;
    int bookId;
    int strcmpPerson = -1;
    int strcmpBook = -1;
    cout << "Please enter the card ID: ";
    cin >> cardholderId;

    for(int i = 0; i < cardholders.size(); i++) {
      if(cardholderId == cardholders.at(i)->getId()) {strcmpPerson = 0; cardIndex = i; break;}
    }
    if(strcmpPerson == -1) { cout << "Card ID not found " << endl; return;}
    if(cardholders.at(cardIndex)->isActive() == false) {cout << "Card not active. Please activate your card." << endl; return;}
    cout << "Cardholder: " << cardholders.at(cardIndex)->fullName() << endl;
    cout << "Please enter the book ID: ";
    cin >> bookId;
    cout << endl;
    for(int i = 0; i < books.size(); i++) {
      if(bookId == books.at(i)->getId()) {strcmpBook = 0; bookIndex = i; break;}
    }
    if(strcmpBook == -1) { cout << "Book ID not found " << endl; return;}
    if(books.at(bookIndex)->getPersonPtr() != nullptr) { cout << "Book is already checked out." << endl; return;}
    // we need to set the books personPtr to point to the person that checked it out
    Person *ptr = cardholders.at(cardIndex);
    books.at(bookIndex)->setPersonPtr(ptr);

    ofstream outData;
    outData.open("rentals.txt");
    Person *rentals;
    for(int i = 0; i < books.size(); i++) {
      rentals = books.at(i)->getPersonPtr();
      if(rentals != nullptr) { outData << books.at(i)->getId() << " " << (*rentals).getId() << endl;}
    }
    ptr = nullptr;
    rentals = nullptr;
    cout << "Rental Complete" << endl;
    outData.close();

}
// turn the book's (that they are returning)(make sure its a valid book in our book.txt) setPersonPtr() to nullptr
// update rentals.txt (remove the book they return) same as bookcheckout
void bookReturn(vector<Book *> &books) {
  int bookId;
  cout << "Please enter the book ID to return: ";
  cin >> bookId;
  cout << endl;
  int bookIndex;
  int strcmpBook = -1;
  for(int n = 0; n < books.size(); n++) {
    if(bookId == books.at(n)->getId()) {strcmpBook = 0; bookIndex = n; break;}
  }
  if(strcmpBook == -1) { cout << "Book ID not found " << endl; return;}
  if(books.at(bookIndex)->getPersonPtr() == nullptr) { cout << "Book is already returned." << endl; return;}
  books.at(bookIndex)->setPersonPtr(nullptr);
  ofstream outData;
  outData.open("rentals.txt");
  Person *ptr;
  for(int i = 0; i < books.size(); i++) {
    ptr = books.at(i)->getPersonPtr();
    if(ptr != nullptr) { outData << books.at(i)->getId() << " " << (*ptr).getId() << endl;}
  }
  ptr = nullptr;
  cout << "Tital: " << books.at(bookIndex)->getTitle() << endl;
  cout << "Return complete" << endl;
  outData.close();
}

// go through books vector and if their personPtr == nullptr cout the books info
void viewAllAvBooks(vector<Book *> &books) {
  int check = 1;
  for(int i = 0; i < books.size(); i++) {
      if(books.at(i)->getPersonPtr() == nullptr) {
        cout << "Book ID: " << books.at(i)->getId() << endl;
        cout << "Title: " << books.at(i)->getTitle() << endl;
        cout << "Author: " << books.at(i)->getAuthor() << endl;
        cout << "Category: " << books.at(i)->getCategory() << endl;
        cout << endl;
        check = 0;
      }
    }
    if(check == 1) {cout << "No available books." << endl;}
}
// go through books vector and if the personPtr is not nullptr then cout the books info and the persons name and cardId who checked the book out
void viewAllOutstandingRentals(vector<Book *> &books) {
  int check = 1;
  for(int i = 0; i < books.size(); i++) {
    if(books.at(i)->getPersonPtr() != nullptr) {
      cout << "Book ID: " << books.at(i)->getId() << endl;
      cout << "Title: " << books.at(i)->getTitle() << endl;
      cout << "Author: " << books.at(i)->getAuthor() << endl;
      cout << "Category: " << books.at(i)->getCategory() << endl;
      cout << endl;
      check = 0;
    }
  }
  if(check == 1) {cout << "No outstanding rentals." << endl;}
}
// go through books
// if that books personPtr is pointing to the same person cout that books info
void viewPersonsRentals(vector<Person *> &cardholders, vector<Book *> &books) {
  int cardId;
  cout << "Please enter card ID: ";
  cin >> cardId;
  int cardIndex;
  int idComp = -1;
  for(int i = 0; i < cardholders.size(); i++) {
    if(cardId == cardholders.at(i)->getId()) { cardIndex = i; idComp = 0; break;}
  }

  if(idComp == -1) { cout << "Card ID not found." << endl; return;}
  if(cardholders.at(cardIndex)->isActive() == false) { cout << "Card not active. Please activate your card."; return;}
  Person *ptr;
  int check = 1;
  for(int i = 0; i < books.size(); i++) {
    ptr = books.at(i)->getPersonPtr();
    if(ptr != nullptr && (*ptr).getId() == cardId) {
      cout << books.at(i)->getId() << endl;
      cout << books.at(i)->getTitle() << endl;
      cout << books.at(i)->getAuthor() << endl;
      cout << endl;
      check = 0;
    }
  }
  ptr = nullptr;
  if(check == 1) {cout << "No books currently checked out." << endl;}
}
// add info into our txts of the info that is added
void openCard(vector<Person *> &cardholders, int nextID) {
  string fName;
  string lName;
  bool act = true;
  cout << "Please enter your first name: ";
  cin >> fName;
  cout << endl;
  cout << "Please enter your last name: ";
  cin >> lName;
  cout << endl;
  Person *ptr = new Person(nextID, act, fName, lName);
  cardholders.push_back(ptr);
  // update person.txt
  ofstream outData;
  outData.open("persons.txt");
  for(int i = 0; i < cardholders.size(); i++) {
    outData << cardholders.at(i)->getId() << " " << cardholders.at(i)->isActive() << " " << cardholders.at(i)->fullName() << endl;
    //cout << cardholders.at(i)->getId() << " " << cardholders.at(i)->isActive() << " " << cardholders.at(i)->fullName() << i << endl;
  }
  outData.close();
}
void closeCard(vector<Person *> &cardholders) {
  int cardId;
  cout << "Please enter card ID: ";
  cin >> cardId;
  int cardIndex;
  int idComp = -1;
  for(int i = 0; i < cardholders.size(); i++) {
    if(cardId == cardholders.at(i)->getId()) { cardIndex = i; idComp = 0; break;}
  }

  if(idComp == -1) { cout << "Card ID not found." << endl; return;}
  if(cardholders.at(cardIndex)->isActive() == false) { cout << "Cardholder: " << cardholders.at(cardIndex)->fullName() << endl << "Card ID is already inactive."; return;}
  cout << "Cardholder: " << cardholders.at(cardIndex)->fullName() << endl;
  cout << "Are you sure you want to deactivate your card? (y/n)";
  string choice;
  cin >> choice;
  if(choice == "y") { cardholders.at(cardIndex)->setActive(0); cout << "Card ID deactivated" << endl;}
  if(choice == "n") {return;}
  ofstream outData;
  outData.open("persons.txt");
  for(int i = 0; i < cardholders.size(); i++) {
    outData << cardholders.at(i)->getId() << " " << cardholders.at(i)->isActive() << " " << cardholders.at(i)->fullName() << endl;
  }
  outData.close();
}
//
// Book * searchBook(vector<Book *> myBooks, int id) {
//     return nullptr;
// }


int main()
{
    vector<Book *> books;
    vector<Person *> cardholders;

    readBooks(books);
    int nextID = readPersons(cardholders);
    readRentals(books, cardholders);
    int choice;
    do
    {
        // If you use cin anywhere, don't forget that you have to handle the <ENTER> key that
        // the user pressed when entering a menu option. This is still in the input stream.
        printMenu();
        cin >> choice;
        switch(choice)
        {
            case 1:
                bookCheckout(books, cardholders);
                break;

            case 2:
                bookReturn(books);
                break;

            case 3:
                viewAllAvBooks(books);
                break;

            case 4:
                viewAllOutstandingRentals(books);
                break;

            case 5:
                viewPersonsRentals(cardholders, books);
                break;

            case 6:
                openCard(cardholders, nextID);
                nextID++;
                break;

            case 7:
                closeCard(cardholders);
                break;

            case 8:
                break;

            default:
                cout << "Invalid entry" << endl;
                break;
        }
        cout << endl;
   } while(choice != 8);

   // THINGS I NEED TO delete
   // anything with new
   // we also want to set our pointers to null/delete?
   Person *personPtr;
   for (int i = 0; i < cardholders.size(); ++i)
   {
     personPtr = cardholders.at(i);
     delete personPtr;
   }
   personPtr = nullptr;
   Book *bookPtr;
   for (int i = 0; i < books.size(); ++i)
   {
     bookPtr = books.at(i);
     delete bookPtr;
   }
   bookPtr = nullptr;
      return 0;
}
