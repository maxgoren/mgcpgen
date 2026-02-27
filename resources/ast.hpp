#ifndef ast_hpp
#define ast_hpp

struct AST {
    virtual ~AST() = default;
    virtual void print() = 0;
    virtual int eval() = 0;
};

#endif