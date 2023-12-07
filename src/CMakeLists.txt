target_sources(
        talos_lib
        PUBLIC
        talos.h
        return_code.h
        expected.h
        source_location.h
        PRIVATE
        talos.cpp
        token.h token.cpp
        frontend/lexer.h frontend/lexer.cpp
        frontend/ast.h frontend/ast.cpp
        frontend/parser.h frontend/parser.cpp
)
