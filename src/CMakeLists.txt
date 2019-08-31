
find_package(FLEX REQUIRED)
flex_target(lexer flex/1.lex  ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cpp
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/clexer.h
)

set(APP compiler)
include_directories(${APP} PUBLIC include ${CMAKE_CURRENT_BINARY_DIR})
add_executable(${APP} ${APP}.cpp CodeGenerator.cpp flexer.cpp parser.cpp SymbolTable.cpp ${FLEX_lexer_OUTPUTS})