TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    lib/base/arithmetic.cpp \
    lib/base/predicate.cpp \
    eval.cpp \
    exceptions.cpp \
    mm_ptr.cpp \
    parse.cpp \
    repl.cpp \
    tokenizer.cpp \
    lib/base/pairlist.cpp \
    tools.cpp \
    MemoryManager.cpp \
    LispCell.cpp \
    Environment.cpp \
    Number.cpp \
    ObjectIndex.cpp \
    Procedure.cpp

HEADERS += \
    lib/base.hpp \
    lib/base/arithmetic.hpp \
    lib/base/predicate.hpp \
    eval.hpp \
    exceptions.hpp \
    mm_ptr.hpp \
    parse.hpp \
    repl.hpp \
    tokenizer.hpp \
    lib/base/pairlist.hpp \
    tools.hpp \
    types/Bool.hpp \
    lib/FuncTable.hpp \
    types/Dict.hpp \
    types/Environment.hpp \
    types/Number.hpp \
    types/Pair.hpp \
    types/Procedure.hpp \
    types/Special.hpp \
    types/String.hpp \
    LispTypeFlag.hpp \
    lispTypes.hpp \
    MemoryManager.hpp \
    ObjectIndex.hpp \
    types/LispCell.hpp
