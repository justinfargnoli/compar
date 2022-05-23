#include <iostream>
#include <memory>
#include <vector>

namespace ast {
    enum class Type {
        Boolean,
        Number
    };

    class Statement {};

    class Declaration : public Statement {
        std::string name;
        Type type;
    };

    class Expression {
    public:
        virtual std::string toString() const = 0;

        virtual bool operator==(const Expression &other) const = 0;

        bool operator!=(const Expression &other) const { return !(*this == other); }
    };
    std::ostream &operator<<(std::ostream &out, const Expression &ast) {
        return out << ast.toString();
    }

    class BooleanLiteral : public Expression {
    public:
        const bool value;

        explicit BooleanLiteral(const bool value) : value(value) {};

        std::string toString() const final {
            return std::to_string(value);
        }

        bool operator==(const Expression &other) const final {
            if (const auto *const other_cast = dynamic_cast<const BooleanLiteral *const>(&other)) {
                return this->value == other_cast->value;
            } else {
                return false;
            }
        }
    };

    class NumberLiteral : public Expression {
    public:
        const unsigned int value;

        explicit NumberLiteral(const unsigned int value) : value(value) {};

        std::string toString() const final {
            return std::to_string(value);
        }

        bool operator==(const Expression &other) const final {
            if (const auto *const other_cast = dynamic_cast<const NumberLiteral *const>(&other)) {
                return this->value == other_cast->value;
            } else {
                return false;
            }
        }
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

    class AST {
        std::vector<Function> functions;
    };
}

namespace parser {
    class Parser { ;
        virtual std::unique_ptr<ast::Expression> parse() = 0;

    protected:
        const std::string &input;
        unsigned int position;

        explicit Parser(const std::string &input) : input(input), position(0) {}
    };

    class BooleanLiteral : Parser {
    public:
        explicit BooleanLiteral(const std::string &input) : Parser(input) {}

        std::unique_ptr<ast::Expression> parse() final {
            std::string trueLiteral{"true"};
            std::string falseLiteral{"false"};

            if (this->input.find(trueLiteral, this->position) == this->position) {
                position += trueLiteral.length();
                return std::make_unique<ast::BooleanLiteral>(true);
            } else if (input.find(falseLiteral, position) == position) {
                position += falseLiteral.length();
                return std::make_unique<ast::BooleanLiteral>(false);
            }

            return nullptr;
        }
    };
}

namespace test {
    void assertEqual(std::unique_ptr<ast::Expression> lhs, std::unique_ptr<ast::Expression> rhs) {
        std::string lhsString{};
        std::string rhsString{};

        if (lhs == rhs) {
            return;
        } else if (!lhs) {
            lhsString = "nullptr";
            rhsString = rhs->toString();
        } else if (!rhs) {
            lhsString = lhs->toString();
            rhsString = "nullptr";
        } else if(*lhs == *rhs) {
            return;
        } else {
            lhsString = lhs->toString();
            rhsString = rhs->toString();
        }

        std::cerr << "NOT EQUAL: " << std::endl << "\tLeft hand side: (" << lhsString << ")" << std::endl
                  << "\tRight hand side: (" << rhsString << ")"
                  << std::endl;
    }

    void parseBooleanLiteral() {
        assertEqual(parser::BooleanLiteral{"true"}.parse(), std::make_unique<ast::BooleanLiteral>(true));
        assertEqual(parser::BooleanLiteral{"false"}.parse(), std::make_unique<ast::BooleanLiteral>(false));
        assertEqual(parser::BooleanLiteral{""}.parse(), nullptr);
        assertEqual(parser::BooleanLiteral{"tru"}.parse(), nullptr);
        assertEqual(parser::BooleanLiteral{"alse"}.parse(), nullptr);
        assertEqual(parser::BooleanLiteral{"asdf"}.parse(), nullptr);
    }

    void test() {
        parseBooleanLiteral();
    }
}

int main() {
    test::test();
    return 0;
}
