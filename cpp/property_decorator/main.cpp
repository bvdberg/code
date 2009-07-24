#include <iostream>
#include <string>
#include <map>

using namespace std;

class Property {
public:
};


class ItemProperty : public Property {
public:
    ItemProperty(int weight_) : weight(weight_) {}
    int weight;     
};


class EquipProperty : public ItemProperty {
public:
    EquipProperty(int weight_, string slot_) : ItemProperty(weight_), slot(slot_) {}
    string slot;
};


class Object {
public:
    
    Object(string name_) : name(name_) {}
    void addProperty(string propname, Property* prop) {
        properties[propname] = prop;
    }
    bool hasProperty(string propname) const {
        PropertiesIter iter = properties.find(propname);
        return (iter != properties.end());
    }
    const Property* getProperty(string propname) const {
        PropertiesIter iter = properties.find(propname);
        if (iter != properties.end()) return iter->second;
        return 0;
    }
    string name;

private:
    typedef std::map<string, Property*> Properties;
    typedef Properties::const_iterator PropertiesIter;
    Properties properties;
};


#define CHECK_PROPERTY(x, msg) \
    if (!obj.hasProperty(x)) { cout << msg; return; } 


void pickup(Object& obj) {
    cout << ">> pickup '" << obj.name << "'\n";
    CHECK_PROPERTY("item", "you cannot pick that up!\n");

    ItemProperty* itemProp = (ItemProperty*)obj.getProperty("item");
    cout << "  item " << obj.name << " weight: " << itemProp->weight << "\n";
}


void wear(Object& obj) {
    cout << ">> wear '" << obj.name << "'\n";
    CHECK_PROPERTY("equip", "you cannot wear that\n");
    EquipProperty* equipProp = (EquipProperty*)obj.getProperty("equip");
    cout << "  equip " << obj.name << " weight: " << equipProp->weight << " slot: " << equipProp->slot << "\n";
}


int main() {
    Object* obj = new Object("object");

    Object* item = new Object("item");
    item->addProperty("item", new ItemProperty(10));

    Object* equip = new Object("equip");
    // NASTY: we need a way to make a property hierarchy
    equip->addProperty("item", new ItemProperty(10));
    equip->addProperty("equip", new EquipProperty(10, "leg"));

    pickup(*obj);
    pickup(*item);
    pickup(*equip);

    wear(*obj);
    wear(*item);
    wear(*equip);

    return 0;
}

