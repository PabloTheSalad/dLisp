#include "specialForms.h"
#include "tools.hpp"
#include "exceptions.hpp"
#include "eval.hpp"


/*!
 * \brief Преобразует переданную ей форму let выражения в эквивалентное
 * lambda - выражение
 * \param args Аргументы let формы
 * \return lambda - выражение
 *
 * Стоит отметить что в данной функции принимается за истину правильность
 * переданной формы let и никаких проверок не производится
 */
obj_ptr letMacro(obj_ptr args) {
    obj_ptr fargs;
    obj_ptr values;
    obj_ptr argsList = args->at(0);
    if (argsList->type == T_EMPTY) {
        fargs = emptyList();
        values = emptyList();
    } else {
//        forAllInList(argsList, [&fargs, &values](auto pair){
//            if (!fargs.isValid()) fargs = singletonList(pair->at(0));
//            else fargs->append(pair->at(0));
//            if (!values.isValid()) values = singletonList(pair->at(1));
//            else values->append(pair->at(1));
//        });
        for (auto cell : *argsList) {
            if (!fargs.isValid()) fargs = singletonList(cell->at(0));
            else fargs->append(cell->at(0));
            if (!values.isValid()) values = singletonList(cell->at(1));
            else values->append(cell->at(1));
        }
    }
    obj_ptr lambda = singletonList(makeSymbol("lambda"));
    lambda->append(fargs);
    lambda->pair().cdr->pair().cdr = args->pair().cdr;
    return makePair(lambda, values);
}
