#ifndef SEMANTIC_UTILITIES_H
#define SEMANTIC_UTILITIES_H

#include"lexer.h"
#include"string_utilities.h"

#include<re2/re2.h>
#include<re2/stringpiece.h>

#include<string>
#include<memory>
#include<any>

template<typename T>
bool is_public(const T& t) {
    return t.access == nullptr || *t.access == PUBLIC_K;
};

// EC de escaped char
// NS de Numeric char
enum escaped_sequence {
    ESCAPE_CHAR_EC = 0,
    SINGLE_QUOTE_EC,
    DOUBLE_QUOTE_EC,
    QUESTION_MARK_EC,
    AUDIBLE_BELL_EC,
    BACK_SPACE_EC,
    NEW_PAGE_EC, // FORM_FEED_EC
    NEW_LINE_EC, // LINE_FEED_EC
    HORIZONTAL_TAB_EC,
    VERTICAL_TAB_EC,
    OCT_VALUE_NC,
    HEX_VALUE_NC,
    NO_ESCAPED_SEQUENCE,
};

bool is_escaped_char(escaped_sequence es) {
    return es == ESCAPE_CHAR_EC
        || es == SINGLE_QUOTE_EC
        || es == DOUBLE_QUOTE_EC
        || es == QUESTION_MARK_EC
        || es == AUDIBLE_BELL_EC
        || es == BACK_SPACE_EC
        || es == NEW_LINE_EC
        || es == NEW_PAGE_EC
        || es == HORIZONTAL_TAB_EC
        || es == VERTICAL_TAB_EC;
}

bool is_numeric_char(escaped_sequence es) {
    return es == OCT_VALUE_NC
        || es == HEX_VALUE_NC;
}

struct string_literal_parser {
    std::unique_ptr<RE2> expression;
    std::array<std::pair<std::string,std::any>, NO_ESCAPED_SEQUENCE + 1> replacements;
    
    string_literal_parser(const std::string& escape = R"(\)") {
        using namespace std::string_literals;
        replacements[ESCAPE_CHAR_EC]      = { RE2::QuoteMeta(escape + escape), escape};
        replacements[SINGLE_QUOTE_EC]     = { RE2::QuoteMeta(escape + R"(')"), "\'"s};
        replacements[DOUBLE_QUOTE_EC]     = { RE2::QuoteMeta(escape + R"(")"), "\""s};
        replacements[QUESTION_MARK_EC]    = { RE2::QuoteMeta(escape + R"(?)"), "\?"s};
        replacements[AUDIBLE_BELL_EC]     = { RE2::QuoteMeta(escape + R"(a)"), "\a"s};
        replacements[BACK_SPACE_EC]       = { RE2::QuoteMeta(escape + R"(b)"), "\b"s};
        replacements[NEW_PAGE_EC]         = { RE2::QuoteMeta(escape + R"(f)"), "\f"s};
        replacements[NEW_LINE_EC]         = { RE2::QuoteMeta(escape + R"(n)"), "\n"s};
        replacements[HORIZONTAL_TAB_EC]   = { RE2::QuoteMeta(escape + R"(t)"), "\t"s};
        replacements[VERTICAL_TAB_EC]     = { RE2::QuoteMeta(escape + R"(v)"), "\v"s};
        replacements[OCT_VALUE_NC]        = { RE2::QuoteMeta(escape) + R"([0-3]?[0-7]{1,2})", 8u}; // Los numericos tienen recursos especiales para reemplazarlos, no es un simple reemplazo como los demas
        replacements[HEX_VALUE_NC]        = { RE2::QuoteMeta(escape) + R"(x[[:xdigit:]]{1,2})", 16u};
        replacements[NO_ESCAPED_SEQUENCE] = { R"([^)" + RE2::QuoteMeta(escape) + R"(]+)", nullptr};
        auto obtain_form = [](const std::pair<std::string,std::any>& p) {return p.first;};
        expression = std::make_unique<RE2>(transform_join(replacements,obtain_form,")|(","(",")"));
    };
    
    std::string parse(const token& t) {
        re2::StringPiece input(t.source);
        std::array< re2::StringPiece, NO_ESCAPED_SEQUENCE + 1 > mt;
        std::array< RE2::Arg,         NO_ESCAPED_SEQUENCE + 1 > args;
        std::array< const RE2::Arg*,  NO_ESCAPED_SEQUENCE + 1 > m;
        std::transform(mt.begin( ),   mt.end( ),   args.begin( ), [](re2::StringPiece& s){return &s;});
        std::transform(args.begin( ), args.end( ), m.begin( ),    [](RE2::Arg& a)        {return &a;});
        
        std::string res = "";
        while(!input.empty()) {
            if( !RE2::FindAndConsumeN(&input, *expression, m.begin( ), m.size( )) ) {
                using namespace std::string_literals;
                throw std::make_pair( t, "Semantic Error: String literal is ill-formed"s);
            }
            escaped_sequence seq_type = escaped_sequence(std::find_if(mt.begin( ), mt.end( ),[](const re2::StringPiece& s){
                return !s.empty( );
            }) - mt.begin( ));
            if(is_escaped_char(seq_type)) {
                res += std::any_cast<std::string>(replacements[seq_type].second);
            }
            else if(is_numeric_char(seq_type)) {
                std::string s = "0" + mt[seq_type].substr(1).as_string();
                res += char(std::stoul(s,nullptr,std::any_cast<unsigned>(replacements[seq_type].second)));
            }
            else {
                res += mt[seq_type].as_string();
            }
        }
        return res.substr(1,res.size() - 2);
    };
};

#endif
