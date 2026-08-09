#ifndef object_hpp
#define object_hpp
#include <iostream>
#include <deque>
#include "../parse/ast.hpp"
using namespace std;

enum ObjectType {
    NUMBER, STRING, BOOL, LIST, FUNCTION, NIL
};

struct Function;

struct Object {
    ObjectType type;
    union {
        bool           boolval;
        double         numval;
        string*        strval;
        deque<Object>* listval;
        Function*      funcval;
    };
    Object(bool v) : boolval(v), type(BOOL) { }
    Object(double dv) : numval(dv), type(NUMBER) { }
    Object(string* str) : strval(str), type(STRING) { }
    Object(deque<Object>* lv) : listval(lv), type(LIST) { }
    Object(Function* fv) : funcval(fv), type(FUNCTION) { }
    Object() : type(NIL) { }
    Object(const Object& ob) {
        type = ob.type;
        switch (type) {
            case STRING: strval = ob.strval; break;
            case NUMBER: numval = ob.numval; break;
            case LIST: listval = ob.listval; break;
            case FUNCTION: funcval = ob.funcval; break;
            case BOOL: boolval = ob.boolval; break;
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

#endif