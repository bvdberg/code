#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <list>

/*
 2 Options:
    pure virtual classes (Object, Item, Food)
        + no overhead in attributes
        - need to reimplement all virtual methods every time
    template at every level
        - additional template overhead (ItemTemplate, FoodTemplate)
    keep template at Item level and downcast
        + easy (only one pointer)
        + only few virtual methods needed
        - downcast (will always succeed)
    proxyItems
        - need to reimplement whole interface
        + only need one pointer

    OR: skip everything, std::strings are shared anyways

    MudObject: 44 bytes
        common: name(4+), descr(4+), aliases(12+)   = 20+
        unique: visible(4), room(4), listeners(8+), triggers(8+) = 24+
    MudItem:   52 bytes
        common: Object(20+), type(4+), weight(4+) = 28+
        unique: Object(24+)
    MudConsumable: 56 bytes
        common: Item(28+), effect(4) = 32+
        unique: Object(24+)
    MudWearable: 68 bytes
        common: Item(28)+), Slot(4), modifiers(12+) = 44+
        unique: Object(24+)         NOTE: no unique modifiers

    MudMobile: 84 bytes
        common: Object(20+), base stats(16) = 32+
        unique: Object(24+), modified stats(16), health(4), inCombat(4), target(4) = 52+
    MudCreature: 108 bytes
        common: MudMobile(32+), aggressive(4), messages(12+), chat(8) = 56+
        unique: MudMobile(52+)
 */

using namespace std;

class Error : public std::exception {
public:
    Error(const std::string& m) : msg(m) {}
    virtual ~Error() throw() {}
    virtual const char* what() const throw() { return msg.c_str(); }
private:
    std::string msg;
};

class ItemTemplate {
public:
    string name;
    string descr;
    int weight;
    int id;
    ItemTemplate(string n, string d, int w, int id_)
            : name(n), descr(d), weight(w), id(id_) {}
};


class EquipTemplate : public ItemTemplate {
public:
    string slot;
    EquipTemplate(string n, string d, int w, int id_, string s)
            : ItemTemplate(n, d, w, id_), slot(s) {}
};


class Object {
public:
    Object(string name_, string descr_) : name(name_) , descr(descr_) {} 
    virtual ~Object() {}
    virtual const string& getName() const { return name; }
    virtual const string& getDescr() const { return descr; }
private:
    string name;
    string descr;
};


class Item : public Object {
public:
    Item(const ItemTemplate* templater) : Object("", ""), template_(templater) {}
    virtual const string& getName() const { return template_->name; }
    virtual const string& getDescr() const { return template_->descr; }
    int getWeight() const { return template_->weight; }
    int getId() const { return template_->id; }
protected:
    const ItemTemplate* template_;
};


class Equip : public Item {
public:
    Equip(const EquipTemplate* templater) : Item(templater) {}
    const string& getSlot() const {
        return ((EquipTemplate*)template_)->slot;       // DOWNCAST
    }
};


/*
class ProxyItem : public Item {
public:
    ProxyItem(const Item& item_) : item(item_) {}
    virtual const string& getName() const { return item.name; }
    virtual const string& getDescr() const { return item.descr; }
    virtual int getWeight() const { return item.weight; }
    virtual int getId() const { return item.id; }
private:
    const Item& item;   // no ownership
};


class ProxyEquip : public Equip {
    ProxyEquip(const Equip& equip_) : equip(equip_) {}
    virtual const string& getName() const { return equip.name; }
    virtual const string& getDescr() const { return equip.descr; }
    virtual int getWeight() const { return equip.weight; }
    virtual int getId() const { return equip.id; }
    virtual const string& getSlot() const { return equip.slot; }
};
*/

int main() {
    // loading phase
    ItemTemplate* template1 = new ItemTemplate("rock", "it's a rock", 10, 123);
    EquipTemplate* template2 = new EquipTemplate("hamburger", "A huge Bigmac", 2, 14, "leg");


    // usage phase
    Item* item = new Item(template1);
    Equip* equip = new Equip(template2);

    typedef std::list<void*> Mylist;
    Mylist mylist;
    cout << "list = " << sizeof(mylist) << endl;

    typedef std::map<int, void*> Mymap;
    Mymap mymap;
    cout << "map = " << sizeof(mymap) << endl;

    cout << "vector = " << sizeof(std::vector<void*>) << endl;

    return 0;
}

