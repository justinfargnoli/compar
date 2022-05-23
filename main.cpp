#include <iostream>
#include <memory>
#include <vector>

namespace ast {
    enum class Type {
        Boolean,
        Number
    };

    class Statement {
    };

    class Declaration : public Statement {
        std::string name;
        Type type;
    };

    class Expression {
    };

    class BooleanLiteral : public Expression {
    public:
        const bool value;

        explicit BooleanLiteral(const bool value) : value(value) {};
    };

    class NumberLiteral : public Expression {
    public:
        const unsigned int value;

        explicit NumberLiteral(const unsigned int value) : value(value) {};
    };

    class Assignment : public Statement {
        std::string name;
        std::unique_ptr<Expression> expression;
    };

    class Function {
        std::vector<std::pair<std::string, Type>> parameters;
        Type returnType;
        std::vector<Statement> body;
    };

    class File {
        std::vector<Function> functions;
    };
}

namespace parser {
    class Parser {
    public:
        virtual bool parse(const std::string &input, unsigned int &position) const = 0;
    };

    class Literal : public Parser {
        const std::string &literal;;

    public:
        bool parse(const std::string &input, unsigned int &position) const final {
            if (input.find(literal, position) == position) {
                position += literal.length();
                return true;
            }
            return false;
        }

        explicit Literal(const std::string &literal) : literal(literal) {}
    };

    class Either : public Parser {
        const std::vector<Parser> &parsers;;

        bool parse(const std::string &input, unsigned int &position) const final {
            for (const auto &parser : parsers) {
                if (parser.parse(input, position)) {
                    return true;
                }
            }
            return false;
        }

    public:
        explicit Either(const std::vector<Parser> &parsers) : parsers(parsers) {}
    };

    class Repeat : public Parser {
        const Parser &repeating;
        const Parser &seperatedBy;;

    public:
        bool parse(const std::string &input, unsigned int &position) const final {
            while (repeating.parse(input, position) && seperatedBy.parse(input, position)) {}
            return true;
        }

        explicit Repeat(const Parser &repeating, const Parser &seperatedBy) : repeating(repeating),
                                                                                    seperatedBy(seperatedBy) {}
    };

    class Whitespace : public Parser {
        bool parse(const std::string &input, unsigned int &position) const final {
            Repeat(Either{{Literal{" "}, Literal{"\t"}, Literal{"\n"}}}, Literal{""})
        }
    };
}

namespace test {
//    void assertEqual(std::unique_ptr<ast::Expression> lhs, std::unique_ptr<ast::Expression> rhs) {
//        std::string lhsString{};
//        std::string rhsString{};
//
//        if (lhs == rhs) {
//            return;
//        } else if (!lhs) {
//            lhsString = "nullptr";
//            rhsString = rhs->toString();
//        } else if (!rhs) {
//            lhsString = lhs->toString();
//            rhsString = "nullptr";
//        } else if(*lhs == *rhs) {
//            return;
//        } else {
//            lhsString = lhs->toString();
//            rhsString = rhs->toString();
//        }
//
//        std::cerr << "NOT EQUAL: " << std::endl << "\tLeft hand side: (" << lhsString << ")" << std::endl
//                  << "\tRight hand side: (" << rhsString << ")"
//                  << std::endl;
//    }
//
//    void parseBooleanLiteral() {
//        assertEqual(parser::BooleanLiteral{"true"}.parse(), std::make_unique<ast::BooleanLiteral>(true));
//        assertEqual(parser::BooleanLiteral{"false"}.parse(), std::make_unique<ast::BooleanLiteral>(false));
//        assertEqual(parser::BooleanLiteral{""}.parse(), nullptr);
//        assertEqual(parser::BooleanLiteral{"tru"}.parse(), nullptr);
//        assertEqual(parser::BooleanLiteral{"alse"}.parse(), nullptr);
//        assertEqual(parser::BooleanLiteral{"asdf"}.parse(), nullptr);
//    }

    void test() {
//        parseBooleanLiteral();
    }
}

int main() {
    test::test();
    return 0;
}
