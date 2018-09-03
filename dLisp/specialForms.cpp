#include "specialForms.h"
#include "tools.hpp"
#include "exceptions.hpp"
#include "eval.hpp"

obj_ptr letMacro(obj_ptr args) {
    obj_ptr fargs;
    obj_ptr values;
    obj_ptr argsList = args->at(0);
    if (argsList->type == T_EMPTY) {
        fargs = emptyList();
        values = emptyList();
    } else {
        forAllInList(argsList, [&fargs, &values](auto pair){
            if (fargs.isNull()) fargs = singletonList(pair->at(0));
            else fargs->append(pair->at(0));
            if (values.isNull()) values = singletonList(pair->at(1));
            else values->append(pair->at(1));
        });
    }
    obj_ptr lambda = singletonList(makeSymbol("lambda"));
    lambda->append(fargs);
    lambda->pair->cdr->pair->cdr = args->pair->cdr;
    return makePair(lambda, values);
}
