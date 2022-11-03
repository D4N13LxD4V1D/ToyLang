#include "ToyLang.hpp"

bool ToyLang::hadError = false;

int main(int argc, char const *argv[]) {
    if (argc > 2)
        std::cerr << "Usage: " << argv[0] << " [script]" << std::endl;
    else if (argc == 2)
        ToyLang::runFile(argv[1]);
    else
        ToyLang::runPrompt();

    return 0;
}

void ToyLang::runFile(const char *path) {
    std::ifstream file(path);
    if (!file.is_open())
        std::cerr << "Could not open file: " << path << std::endl;

    std::stringstream buffer;

    buffer << file.rdbuf();
    file.close();

    run(buffer.str().c_str());

    if (ToyLang::hadError)
        exit(65);
}

void ToyLang::runPrompt() {
    std::string line;
    while (true) {
        std::cout << "\x1b[1;33m>>> \x1b[0m";
        std::getline(std::cin, line);
        if (line == "exit")
            return;

        run(line.c_str());
        ToyLang::hadError = false;
    }
}

void ToyLang::run(const char *source) {
    static std::vector<Token> allTokens;
    Lexer lexer(source);
    auto tokens = lexer.scanTokens();

    if (allTokens.empty())
        allTokens = tokens;
    else {
        allTokens.pop_back();
        allTokens.insert(allTokens.end(), tokens.begin(), tokens.end());
    }

    // Parser<std::string> parser(allTokens);
    // auto expr = parser.parse();

    // if (expr == nullptr)
    //     return;

    // StringifyAST stringifier;
    // std::cout << stringifier.toString(expr) << std::endl;

    Parser<llvm::Value *> parser(allTokens);
    auto expr = parser.parse();

    if (expr == nullptr)
        return;

    Compiler compiler;
    compiler.codegen(expr);

    compiler.print();
}

void ToyLang::error(int line, std::string message) {
    Logger::report(line, "", message);
    ToyLang::hadError = true;
}

void ToyLang::error(Token token, std::string message) {
    if (token.type == TokenType::END_OF_FILE)
        Logger::report(token.line, " at end", message);
    else
        Logger::report(token.line, " at '" + token.lexeme + "'", message);
    ToyLang::hadError = true;
}
