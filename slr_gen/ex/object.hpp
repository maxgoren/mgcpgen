#ifndef object_hpp
#define object_hpp
#include <iostream>
#include <deque>
using namespace std;

enum ObjectType {
    NUMBER, STRING, BOOL, LIST, NIL
};

struct Object {
    ObjectType type;
    union {
        bool           boolval;
        double         numval;
        string*        strval;
        deque<Object>* listval;
    };
    Object(bool v) : boolval(v), type(BOOL) { }
    Object(double dv) : numval(dv), type(NUMBER) { }
    Object(string* str) : strval(str), type(STRING) { }
    Object(deque<Object>* lv) : listval(lv), type(LIST) { }
    Object() : type(NIL) { }
    string toString() {
        switch (type) {
            case STRING: return *strval;
            case NUMBER: return to_string(numval);
            case BOOL:   return boolval ? "true":"false";
            case LIST:   {
                string l = "[ ";
                for (int i = 0;i < listval->size()-1; i++) {
                    l += listval->at(i).toString();
                    l += ", ";
                }
                l += listval->at(listval->size()-1).toString() + " ]";
                return l;
            } break;
            default:
                break;
        }
        return "(nil)";
    }
};



#endif