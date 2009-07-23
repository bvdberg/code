#include <stdio.h>
#include <iostream>
#include <string>

/*
    Conclusion:
    - the Iterator solution is a loop with function call
    - the Visitor solution is also a loop with a function call
    - the difference is that the Iterator function call isn't virtual, so it can be inlined.
    - also the Iterator solution doesn't need a custom Visitor class for each different use.

    -> measured: it does NOT matter. For a collection of 50000, both solutions take around 1 ms!
*/

using namespace std;

class Object {
public:
    Object(string name_) : name(name_) {}
    virtual ~Object() {}
    friend ostream& operator<<(ostream& os,const Object& obj) {
        os << obj.name;
        return os;
    }
    string name;
private:
    Object(const Object& rhs);
    Object& operator=(const Object& rhs);
};

// ---------------------------------------------------------------

template <typename T>
class ConstVisitor {
public:
    virtual ~ConstVisitor() {}
    virtual void handle(const T& obj) = 0;
};


template <typename T>
class Visitor {
public:
    virtual ~Visitor() {}
    virtual bool handle(T& obj) = 0;
};


template <typename T>
struct Entry {
    T* item;
    Entry* next;
    Entry* prev;
    Entry(T* item_) : item(item_), next(0), prev(0) {}
    ~Entry() {
        if (item) delete item;
    }
};

template <typename T> class ConstIterator;

template <typename T>
class Collection {
public:
    Collection() : head(0), tail(0) {}
    ~Collection() {
        while (head) {
            Entry<T>* entry = head->next;
            delete head;
            head = entry;
        }
    }
    void addObject(Object* obj) {
        Entry<T>* newEntry = new Entry<T>(obj);
        if (head) {
            tail->next = newEntry;
            newEntry->prev = tail;
        } else {
            head = newEntry;
        }
        tail = newEntry;
    }
    void removeObject(Object* obj) {
        Entry<T>* entry = head;
        while (entry) {
            if (entry->item == obj) {
                removeEntry(entry);
                return;
            }
            entry = entry->next;
        }
    }
    void visit(ConstVisitor<T>& visitor) const {
        Entry<T>* entry = head;
        while (entry) {
            visitor.handle(*(entry->item));
            entry = entry->next;
        }
    }
    void visit(Visitor<T>& visitor) {
        Entry<T>* entry = head;
        while (entry) {
            bool taken = visitor.handle(*entry->item);
            if (taken) {
                Entry<T>* nextEntry = entry->next;
                removeEntry(entry);
                entry = nextEntry;
            } else {
                entry = entry->next;
            }
        }
    }
private:
    void removeEntry(Entry<T>* entry) {
        // update prev
        if (entry != head) {
            entry->prev->next = entry->next;
        } else {
            head = entry->next;
        }
        // update next
        if (entry != tail) {
            entry->next->prev = entry->prev;
        } else {
            tail = entry->prev;
        }
        entry->item = 0;
        delete entry;
    }
    Entry<T>* head;
    Entry<T>* tail;
    friend class ConstIterator<T>;
    Collection(const Collection& rhs);
    Collection& operator= (const Collection& rhs);
};


template <typename T>
class ConstIterator {
public:
    ConstIterator(Collection<T>& collection_) : collection(collection_), entry(collection.head) {}
    const T* operator*() const {
        if (entry) return entry->item;
        return 0;
    }
    void operator++() {
        if (entry) entry = entry->next; 
    }
    void erase() {
        if (!entry) return;
        Entry<T>* next = entry->next;
        collection.removeEntry(entry);
        entry = next;
    }
private:
    Collection<T>& collection;
    Entry<T>* entry;
};


// ----------------------------------------------------------------

class MyObjectConstVisitor : public ConstVisitor<Object> {
public:
    virtual void handle(const Object& obj) {
        cout << "ConstVisitor: handling " << obj << endl;
    }
};


class MyObjectVisitor : public Visitor<Object> {
    virtual bool handle(Object& obj) {
        cout << "Visitor: handling " << obj << endl;
        if (obj.name == "faa") {
            cout << "  taking " << obj << endl;
            delete &obj;
            return true;    // take it
        }
        return false;
    }
};


int main() {
    Collection<Object> collection;
    collection.addObject(new Object("foo"));
    collection.addObject(new Object("bar"));
    collection.addObject(new Object("faa"));
    collection.addObject(new Object("wii"));

    MyObjectConstVisitor constvisitor;
    collection.visit(constvisitor);

    MyObjectVisitor visitor;
    collection.visit(visitor);

    ConstIterator<Object> iter(collection);
    while (*iter) {
        cout << "iterating " << *(*iter) << endl;
        ++iter;
    }

    return 0;
}

