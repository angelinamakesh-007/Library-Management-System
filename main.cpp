#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <limits>

using namespace std;

// ─────────────────────────────────────────────
//  Utility: get today's date as a string
// ─────────────────────────────────────────────
string today() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    ostringstream oss;
    oss << (now->tm_year + 1900) << "-"
        << setw(2) << setfill('0') << (now->tm_mon + 1) << "-"
        << setw(2) << setfill('0') << now->tm_mday;
    return oss.str();
}

// ─────────────────────────────────────────────
//  Class: Book
// ─────────────────────────────────────────────
class Book {
public:
    int     id;
    string  title;
    string  author;
    string  isbn;
    int     totalCopies;
    int     availableCopies;

    Book(int id, const string& title, const string& author,
         const string& isbn, int copies)
        : id(id), title(title), author(author),
          isbn(isbn), totalCopies(copies), availableCopies(copies) {}

    void display() const {
        cout << left
             << setw(4)  << id
             << setw(28) << title
             << setw(22) << author
             << setw(15) << isbn
             << "Copies: " << availableCopies << "/" << totalCopies << "\n";
    }
};

// ─────────────────────────────────────────────
//  Class: Member
// ─────────────────────────────────────────────
class Member {
public:
    int    id;
    string name;
    string email;
    string phone;
    int    borrowedCount;   // current borrowed books

    Member(int id, const string& name,
           const string& email, const string& phone)
        : id(id), name(name), email(email),
          phone(phone), borrowedCount(0) {}

    void display() const {
        cout << left
             << setw(4)  << id
             << setw(22) << name
             << setw(26) << email
             << setw(14) << phone
             << "Borrowed: " << borrowedCount << "\n";
    }
};

// ─────────────────────────────────────────────
//  Class: BorrowRecord
// ─────────────────────────────────────────────
class BorrowRecord {
public:
    int    recordId;
    int    memberId;
    int    bookId;
    string issueDate;
    string returnDate;   // empty if not yet returned
    bool   returned;

    BorrowRecord(int rid, int mid, int bid, const string& issued)
        : recordId(rid), memberId(mid), bookId(bid),
          issueDate(issued), returnDate(""), returned(false) {}

    void display(const string& memberName, const string& bookTitle) const {
        cout << left
             << setw(4)  << recordId
             << setw(22) << memberName
             << setw(28) << bookTitle
             << setw(12) << issueDate
             << setw(12) << (returned ? returnDate : "---")
             << (returned ? "Returned" : "Issued") << "\n";
    }
};

// ─────────────────────────────────────────────
//  Class: Library  (main controller)
// ─────────────────────────────────────────────
class Library {
private:
    vector<Book>         books;
    vector<Member>       members;
    vector<BorrowRecord> records;

    int nextBookId   = 1;
    int nextMemberId = 1;
    int nextRecordId = 1;

    // ── helpers ──────────────────────────────
    Book* findBook(int id) {
        for (auto& b : books)
            if (b.id == id) return &b;
        return nullptr;
    }

    Member* findMember(int id) {
        for (auto& m : members)
            if (m.id == id) return &m;
        return nullptr;
    }

    string toLower(const string& s) const {
        string r = s;
        transform(r.begin(), r.end(), r.begin(), ::tolower);
        return r;
    }

    void printHeader(const string& title) const {
        cout << "\n╔══════════════════════════════════════════════╗\n";
        cout << "║  " << left << setw(44) << title << "║\n";
        cout << "╚══════════════════════════════════════════════╝\n";
    }

    void printDivider() const {
        cout << string(70, '-') << "\n";
    }

public:
    // ── Seed demo data ────────────────────────
    void seedData() {
        books.emplace_back(nextBookId++, "The Great Gatsby",         "F. Scott Fitzgerald", "978-0743273565", 3);
        books.emplace_back(nextBookId++, "To Kill a Mockingbird",    "Harper Lee",           "978-0061935466", 2);
        books.emplace_back(nextBookId++, "1984",                     "George Orwell",         "978-0451524935", 4);
        books.emplace_back(nextBookId++, "Pride and Prejudice",      "Jane Austen",           "978-0141439518", 2);
        books.emplace_back(nextBookId++, "The Catcher in the Rye",   "J.D. Salinger",         "978-0316769174", 3);

        members.emplace_back(nextMemberId++, "Alice Johnson",  "alice@example.com",  "555-0101");
        members.emplace_back(nextMemberId++, "Bob Smith",      "bob@example.com",    "555-0102");
        members.emplace_back(nextMemberId++, "Carol Williams", "carol@example.com",  "555-0103");
    }

    // ─────────────────────────────────────────
    //  1. Add Book
    // ─────────────────────────────────────────
    void addBook() {
        printHeader("ADD NEW BOOK");
        string title, author, isbn;
        int copies;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Title  : "; getline(cin, title);
        cout << "  Author : "; getline(cin, author);
        cout << "  ISBN   : "; getline(cin, isbn);
        cout << "  Copies : "; cin >> copies;

        if (title.empty() || author.empty()) {
            cout << "  [!] Title and Author cannot be empty.\n";
            return;
        }
        if (copies < 1) copies = 1;

        books.emplace_back(nextBookId++, title, author, isbn, copies);
        cout << "  [✓] Book added with ID " << (nextBookId - 1) << ".\n";
    }

    // ─────────────────────────────────────────
    //  2. Add Member
    // ─────────────────────────────────────────
    void addMember() {
        printHeader("REGISTER NEW MEMBER");
        string name, email, phone;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Name   : "; getline(cin, name);
        cout << "  Email  : "; getline(cin, email);
        cout << "  Phone  : "; getline(cin, phone);

        if (name.empty()) {
            cout << "  [!] Name cannot be empty.\n";
            return;
        }

        members.emplace_back(nextMemberId++, name, email, phone);
        cout << "  [✓] Member registered with ID " << (nextMemberId - 1) << ".\n";
    }

    // ─────────────────────────────────────────
    //  3. Issue Book
    // ─────────────────────────────────────────
    void issueBook() {
        printHeader("ISSUE BOOK");
        int mid, bid;
        cout << "  Member ID : "; cin >> mid;
        cout << "  Book ID   : "; cin >> bid;

        Member* m = findMember(mid);
        Book*   b = findBook(bid);

        if (!m) { cout << "  [!] Member not found.\n"; return; }
        if (!b) { cout << "  [!] Book not found.\n";   return; }

        if (b->availableCopies == 0) {
            cout << "  [!] No copies available for \"" << b->title << "\".\n";
            return;
        }
        if (m->borrowedCount >= 3) {
            cout << "  [!] Member has reached the 3-book borrow limit.\n";
            return;
        }

        // check if member already has this book
        for (auto& r : records) {
            if (r.memberId == mid && r.bookId == bid && !r.returned) {
                cout << "  [!] Member already has this book issued.\n";
                return;
            }
        }

        b->availableCopies--;
        m->borrowedCount++;
        records.emplace_back(nextRecordId++, mid, bid, today());

        cout << "  [✓] \"" << b->title << "\" issued to " << m->name
             << " on " << today() << ".\n";
    }

    // ─────────────────────────────────────────
    //  4. Return Book
    // ─────────────────────────────────────────
    void returnBook() {
        printHeader("RETURN BOOK");
        int mid, bid;
        cout << "  Member ID : "; cin >> mid;
        cout << "  Book ID   : "; cin >> bid;

        Member* m = findMember(mid);
        Book*   b = findBook(bid);

        if (!m) { cout << "  [!] Member not found.\n"; return; }
        if (!b) { cout << "  [!] Book not found.\n";   return; }

        for (auto& r : records) {
            if (r.memberId == mid && r.bookId == bid && !r.returned) {
                r.returned   = true;
                r.returnDate = today();
                b->availableCopies++;
                m->borrowedCount--;
                cout << "  [✓] \"" << b->title << "\" returned by " << m->name
                     << " on " << today() << ".\n";
                return;
            }
        }
        cout << "  [!] No active borrow record found for this member/book.\n";
    }

    // ─────────────────────────────────────────
    //  5. Search Books
    // ─────────────────────────────────────────
    void searchBooks() {
        printHeader("SEARCH BOOKS");
        cout << "  Search by: 1) Title  2) Author  > ";
        int choice; cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Keyword : ";
        string kw; getline(cin, kw);
        string kwL = toLower(kw);

        vector<Book*> results;
        for (auto& b : books) {
            if (choice == 1 && toLower(b.title).find(kwL) != string::npos)
                results.push_back(&b);
            else if (choice == 2 && toLower(b.author).find(kwL) != string::npos)
                results.push_back(&b);
        }

        if (results.empty()) {
            cout << "  No books found matching \"" << kw << "\".\n";
            return;
        }

        printDivider();
        cout << left << setw(4) << "ID"
             << setw(28) << "Title"
             << setw(22) << "Author"
             << setw(15) << "ISBN"
             << "Copies\n";
        printDivider();
        for (auto* b : results) b->display();
        printDivider();
        cout << "  " << results.size() << " result(s) found.\n";
    }

    // ─────────────────────────────────────────
    //  6. List All Books
    // ─────────────────────────────────────────
    void listBooks() {
        printHeader("ALL BOOKS");
        if (books.empty()) { cout << "  No books in the system.\n"; return; }

        printDivider();
        cout << left << setw(4) << "ID"
             << setw(28) << "Title"
             << setw(22) << "Author"
             << setw(15) << "ISBN"
             << "Copies\n";
        printDivider();
        for (auto& b : books) b.display();
        printDivider();
    }

    // ─────────────────────────────────────────
    //  7. List All Members
    // ─────────────────────────────────────────
    void listMembers() {
        printHeader("ALL MEMBERS");
        if (members.empty()) { cout << "  No members registered.\n"; return; }

        printDivider();
        cout << left << setw(4) << "ID"
             << setw(22) << "Name"
             << setw(26) << "Email"
             << setw(14) << "Phone"
             << "Borrowed\n";
        printDivider();
        for (auto& m : members) m.display();
        printDivider();
    }

    // ─────────────────────────────────────────
    //  8. Borrow Records
    // ─────────────────────────────────────────
    void listRecords(bool activeOnly = false) {
        printHeader(activeOnly ? "ACTIVE BORROW RECORDS" : "ALL BORROW RECORDS");
        if (records.empty()) { cout << "  No records found.\n"; return; }

        printDivider();
        cout << left << setw(4)  << "ID"
             << setw(22) << "Member"
             << setw(28) << "Book"
             << setw(12) << "Issued"
             << setw(12) << "Returned"
             << "Status\n";
        printDivider();

        int shown = 0;
        for (auto& r : records) {
            if (activeOnly && r.returned) continue;
            Member* m = findMember(r.memberId);
            Book*   b = findBook(r.bookId);
            string mName = m ? m->name    : "Unknown";
            string bTitle = b ? b->title  : "Unknown";
            r.display(mName, bTitle);
            shown++;
        }
        if (shown == 0) cout << "  No active records.\n";
        printDivider();
    }

    // ─────────────────────────────────────────
    //  Main Menu
    // ─────────────────────────────────────────
    void run() {
        seedData();

        while (true) {
            cout << "\n";
            cout << "╔══════════════════════════════════════════════╗\n";
            cout << "║         📚  LIBRARY MANAGEMENT SYSTEM        ║\n";
            cout << "╠══════════════════════════════════════════════╣\n";
            cout << "║  1. Add Book              6. List All Books   ║\n";
            cout << "║  2. Add Member            7. List All Members ║\n";
            cout << "║  3. Issue Book            8. Active Borrows   ║\n";
            cout << "║  4. Return Book           9. All Records      ║\n";
            cout << "║  5. Search Books          0. Exit             ║\n";
            cout << "╚══════════════════════════════════════════════╝\n";
            cout << "  Choose an option: ";

            int opt;
            if (!(cin >> opt)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            switch (opt) {
                case 1: addBook();              break;
                case 2: addMember();            break;
                case 3: issueBook();            break;
                case 4: returnBook();           break;
                case 5: searchBooks();          break;
                case 6: listBooks();            break;
                case 7: listMembers();          break;
                case 8: listRecords(true);      break;
                case 9: listRecords(false);     break;
                case 0:
                    cout << "\n  Goodbye! 👋\n\n";
                    return;
                default:
                    cout << "  [!] Invalid option. Please try again.\n";
            }
        }
    }
};

// ─────────────────────────────────────────────
//  Entry Point
// ─────────────────────────────────────────────
int main() {
    Library lib;
    lib.run();
    return 0;
}
