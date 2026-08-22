#ifndef object_hpp
#define object_hpp
#include <iostream>
#include <deque>
#include "../parse/ast.hpp"
using namespace std;

enum ObjectType {
    NUMBER, STRING, BOOL, LIST, FUNCTION, NIL, RECORD
};

struct Function;
struct Record;

string record2string(Record* r);

struct Object {
    ObjectType type;
    union {
        bool           boolval;
        double         numval;
        string*        strval;
        deque<Object>* listval;
        Function*      funcval;
        Record*        recordval;
    };
    Object(bool v) : boolval(v), type(BOOL) { }
    Object(double dv) : numval(dv), type(NUMBER) { }
    Object(string* str) : strval(str), type(STRING) { }
    Object(deque<Object>* lv) : listval(lv), type(LIST) { }
    Object(Function* fv) : funcval(fv), type(FUNCTION) { }
    Object(Record* rv) : recordval(rv), type(RECORD) { }
    Object() : type(NIL) { }
    Object(const Object& ob) {
        type = ob.type;
        switch (type) {
            case STRING: strval = ob.strval; break;
            case NUMBER: numval = ob.numval; break;
            case LIST: listval = ob.listval; break;
            case FUNCTION: funcval = ob.funcval; break;
            case BOOL: boolval = ob.boolval; break;
            case RECORD: recordval = ob.recordval; break;
            default:
                break;
        }
    }
    Object& operator=(const Object& ob) {
        if (this != &ob) {
            type = ob.type;
            switch (type) {
                case STRING: strval = ob.strval; break;
                case NUMBER: numval = ob.numval; break;
                case LIST: listval = ob.listval; break;
                case FUNCTION: funcval = ob.funcval; break;
                case BOOL: boolval = ob.boolval; break;
                case RECORD: recordval = ob.recordval; break;
                default:
                    break;
            }
        }
        return *this;
    }
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
            case RECORD: return record2string(recordval);
            default:
                break;
        }
        return "(nil)";
    }
};

struct Frame;

struct Function {
    string name;
    AST* params;
    AST* body;
    Frame* closure;
    Function(string nm, AST* p, AST* b, Frame* f) {
        name = nm;
        params = p;
        body = b;
        closure = f;
    }
};

struct Record {
    string typeName;
    bool instantiated;
    unordered_map<string, Object> fields;
    Record(string tp = "nil", bool alive = false) : typeName(tp), instantiated(alive) { }
};

string record2string(Record* r) {
    string str = r->typeName + "(";
    int i = 0;
    for (auto field : r->fields) {
        str += field.first + " : " + field.second.toString();
        i++;
        if (i < r->fields.size()) str += ", ";
    }
    str += ")";
    return str;
}

#endif