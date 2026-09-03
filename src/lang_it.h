#ifndef LANG_IT_H
#define LANG_IT_H


#include <string>
#include <cstdint>
#include <cstring>
#include <cstddef>
// if you use ETL, set the fixed size of vectors, strings and structure arrays
#define MAX_STRING_LENGTH 80
#define MAX_VECTOR_LENGTH 20
#define MAX_VERB_ENDINGS 12
#define MAX_VERB_CONJUGATIONS 5
#define MAX_ENTRIES 210
#define MAX_NORMALIZATION_RULES 20


#ifdef ETL_BUILD
    #include "etl/string.h"
    #include "etl/vector.h"
    #include "etl/to_arithmetic.h"
    
    using String = etl::string<MAX_STRING_LENGTH>;
    
    using WordString = etl::string<15>;
    using AtomString = etl::string<7>;
    
    template<typename T, size_t N = MAX_VECTOR_LENGTH>
    using Vector = etl::vector<T, N>;

    template<typename T, size_t N>
    using SmallVector = etl::vector<T, N>;


    typedef struct {
        int scope;
        int trigger;
        Vector<AtomString> vowels;
        AtomString insert_vowel;
    } Harmony;

    using HarmonyTable = Vector<Harmony>; 

    typedef struct 
        {
            Vector<AtomString> endings;
            int type;
            int form;
        } VerbRule;

        typedef struct 
        {
            int form;     
            int type;       
            AtomString required_ending;
            AtomString affix;
            uint64_t flags;
            HarmonyTable* vowel_harmony;
        } VerbConjugation;

        


    using VerbRuleDictionary = etl::vector<VerbRule, MAX_VERB_ENDINGS>;
    using VerbConjugationDictionary = etl::vector<VerbConjugation, MAX_VERB_CONJUGATIONS>;
    

    struct MorphVariation {
    uint64_t flag;
    uint64_t required_flags; 
    AtomString ending;
    AtomString form;
    int type;
    int morphology;
    int result_type;
    Vector<int> apply_only_to;
    int stackable;
    HarmonyTable* vowel_harmony;
    int agreement;
};


    inline int string_to_int(const String& str) {
        return etl::to_arithmetic<int>(str.c_str(), str.size());
    }

    typedef struct{
        SmallVector<WordString, 5> source;
        SmallVector<WordString, 5> target;
    } Transfers; 

    
    // Operators
    template<size_t N1, size_t N2>
    auto operator+(const etl::string<N1>& lhs, const etl::string<N2>& rhs) {
        etl::string<N1 + N2> result;
        result = lhs;
        result += rhs;
        return result;
    }
    
    template<size_t N>
    auto operator+(const char* lhs, const etl::string<N>& rhs) {
        etl::string<35> result;
        result = lhs;
        result += rhs;
        return result;
    }
    
    template<size_t N>
    auto operator+(const etl::string<N>& lhs, const char* rhs) {
        etl::string<35> result;
        result = lhs;
        result += rhs;
        return result;
    }



#else
    #include <string>
    #include <vector>
    
    using String = std::string;
    using WordString = std::string;
    using AtomString = std::string;
    
    template<typename T>
    using Vector = std::vector<T>;

    template<typename T>
    using SmallVector = std::vector<T>;

    
    typedef struct {
    int scope;
    int trigger;
   Vector<String> vowels;
   String insert_vowel;
} Harmony;

using HarmonyTable = Vector<Harmony>; 

            typedef struct 
        {
            Vector<String> endings;
            int type;
            int form;
        } VerbRule;

        typedef struct 
        {
            int form;     
            int type;       
            String required_ending;
            String affix;
            uint64_t flags;
            HarmonyTable* vowel_harmony;
        } VerbConjugation;


    using VerbRuleDictionary = std::vector<VerbRule>;
    using VerbConjugationDictionary = std::vector<VerbConjugation>;
    
struct MorphVariation {
    uint64_t  flag;
    uint64_t required_flags; 
    String ending;
    String form;
    int type;
    int morphology;
    int result_type;
    Vector<int> apply_only_to;
    int stackable;
    HarmonyTable* vowel_harmony;
    int agreement;
};

    const String PREV_MARKER = "\u200B";
    inline int string_to_int(const String& str) {
        return std::stoi(str);
    }

    typedef struct{
        Vector<String> source;
        Vector<String> target;
    } Transfers; 

    
#endif
inline bool endsWith(const String& str, const String& suffix) {
    return (str.size() >= suffix.size()) && 
           (str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0);
}

 
#define INIT_REORDER()\
    bool one_ = (i > 0);\
    bool two_ = (i >= 1);\
    bool three_ = (i >= 2);\
    const Word& current = sentence_arr.at(i);\
    const Word* previous = two_ ? &sentence_arr.at(i - 1) : nullptr;\
    const Word* previous_ = three_ ? &sentence_arr.at(i - 2) : nullptr;\

#define CLEANUP(ARR)                                 \
    do {                                             \
        size_t write_idx = 0;                        \
        for (size_t read_idx = 0; read_idx < ARR.size(); ++read_idx) { \
            const Word& w = ARR[read_idx];          \
            bool only_spaces = true;                \
            for (size_t i = 0; i < w.translation.length(); ++i) { \
                if (!isspace(static_cast<unsigned char>(w.translation[i]))) { \
                    only_spaces = false;           \
                    break;                          \
                }                                   \
            }                                       \
            if (!w.translation.empty() && !only_spaces) { \
                ARR[write_idx++] = ARR[read_idx];   \
            }                                       \
        }                                           \
        ARR.resize(write_idx);                       \
    } while(0);

    


#define HOMONYM_OUTCOMES(...) { __VA_ARGS__ }
#define HOMONYM_FORBIDDEN(...) { __VA_ARGS__ }

#define HOMONYM_DEF(word, outcomes_init, forbidden_init, ...) \
static Outcome word##_outcomes[] = outcomes_init; \
static const int word##_forbidden[] = forbidden_init;\
static const char* word##_tokens[] = { __VA_ARGS__ }; 

#define HOMONYM(surface, word) \
{ \
    surface, \
    word##_outcomes, \
    sizeof(word##_outcomes) / sizeof(word##_outcomes[0]), \
    word##_tokens, \
    word##_forbidden, \
    sizeof(word##_tokens) / sizeof(word##_tokens[0]) \
}

enum AFFIX_TYPE {
    PREFIX = 1, 
    SUFFIX = 0,
    PREV_WORD = 2,
    NEXT_WORD = 3,
    INFIX = 4,
    CIRCUMFIX = 5,
    TOTAL_REDUPLICATION = 6,
    PARTIAL_REDUPLICATION_PREFIX = 7, // reduplicated part goes at the beginning
    PARTIAL_REDUPLICATION_SUFFIX = 8, //reduplicated part goes at the end
    PARTIAL_REDUPLICATION_INFIX = 9, // trigger should take the vowel-consonant pattern, but also the position
    MIDDLE_WORD = 10,
    TRANSFIX = 11,
    MUTATION = 12,
    LEXICAL_AFFIX = 13,
    SIMULFIX = 14,
    NONE = 15
};

#define NO_MORPH (Morph*)nullptr
#define NO_CASE (Case*)nullptr
#define NO_VOWEL_HARMONY nullptr
#define MORPH_ALL {}


enum HarmonyScopes {
    STEM = 0
};


enum HarmonyTriggers {
    FIRST_VOWEL = 0,
    LAST_VOWEL = 1,
    FIRST_CONSONANT = 2,
    LAST_CONSONANT = 3,
    LEXICAL = 4

};



typedef struct  {
   int type; // how does it act?
   int word; // what word does it act upon, A or B?
   int order; // A * B or B * A?
   uint64_t flags;
   String addition; 

} GenitiveConstruction;

typedef struct  {
   int type; // how does it act? e.g suffix, prefix, prev word
   String addition; // like "[o] cachorro, [the] dog, hund[en]"
   uint64_t flags; // e.g in swedish EN vs ETT (utrum vs neutrum)
   HarmonyTable* vowel_harmony;
} Definiteness;


enum AdjectiveOrder {
   ADJECTIVE_FIRST = 0,
   NOUN_FIRST = 1
};

typedef struct {
   int clause_order_from;
   int clause_order_to;
   
   GenitiveConstruction gc_from[10]; 
   GenitiveConstruction gc_to[10];   

   int gc_from_count;                
   int gc_to_count;  
   
   AdjectiveOrder ao_from;
   AdjectiveOrder ao_to;

   
   Definiteness def_from[10]; 
   Definiteness def_to[10];   
   
   int def_from_count;                
   int def_to_count;  

   int scripts_from[4];
   int scripts_to[4];
} Info;


inline size_t utf8_length(const String& str) {
    size_t len = 0;
    size_t i = 0;
    while (i < str.size()) {
        unsigned char c = str[i];
        if ((c & 0x80) == 0) i += 1;
        else if ((c & 0xE0) == 0xC0) i += 2;
        else if ((c & 0xF0) == 0xE0) i += 3;
        else if ((c & 0xF8) == 0xF0) i += 4;
        else i += 1;
        len++;
    }
    return len;
}

    
inline uint8_t read_byte(const uint8_t*& ptr) {
    return *ptr++;
}

uint64_t read_u64(const uint8_t*& ptr) {
    uint64_t value = 0;
    for (int i = 0; i < 8; ++i) {
        value |= static_cast<uint64_t>(ptr[i]) << (i * 8);
    }
    ptr += 8;
    return value;
}


        inline AtomString read_string(const uint8_t*& ptr) {
            const char* str = reinterpret_cast<const char*>(ptr);
            AtomString result(str);
            ptr += strlen(str) + 1;
            return result;
        }

inline bool ends_with_multibyte(const String& str, const String& suffix) {
    size_t str_chars = utf8_length(str);
    size_t suffix_chars = utf8_length(suffix);
    
    if (str_chars < suffix_chars) return false;
    
    size_t chars_to_skip = str_chars - suffix_chars;
    size_t byte_pos = 0;
    
    for (size_t i = 0; i < chars_to_skip; i++) {
        unsigned char c = str[byte_pos];
        if ((c & 0x80) == 0) byte_pos += 1;
        else if ((c & 0xE0) == 0xC0) byte_pos += 2;
        else if ((c & 0xF0) == 0xE0) byte_pos += 3;
        else if ((c & 0xF8) == 0xF0) byte_pos += 4;
        else byte_pos += 1;
    }
    
    return str.compare(byte_pos, String::npos, suffix) == 0;
}

inline String remove_ending_multibyte(const String& str, const String& suffix) {
    size_t str_chars = utf8_length(str);
    size_t suffix_chars = utf8_length(suffix);
    
    if (str_chars <= suffix_chars) return "";
    
    size_t chars_to_keep = str_chars - suffix_chars;
    size_t bytes_to_keep = 0;
    size_t chars_kept = 0;
    size_t i = 0;
    
    while (chars_kept < chars_to_keep && i < str.size()) {
        unsigned char c = str[i];
        size_t char_bytes = 0;
        if ((c & 0x80) == 0) char_bytes = 1;
        else if ((c & 0xE0) == 0xC0) char_bytes = 2;
        else if ((c & 0xF0) == 0xE0) char_bytes = 3;
        else if ((c & 0xF8) == 0xF0) char_bytes = 4;
        else char_bytes = 1;
        
        bytes_to_keep += char_bytes;
        i += char_bytes;
        chars_kept++;
    }
    
    return str.substr(0, bytes_to_keep);
}

#define VERB_LOOKUP(DICTIONARY, WORD, REG, DEF, MORPH_FROM, MORPH_TO, USE_MORPH) \
{                                                                       \
    /* First, check for exact matches in verb dictionary */            \
    Verb v = verb_lookup(DICTIONARY, WORD.c_str());                     \
    if (v.t && *v.t) {                             \
        uint64_t flags = v.flags;                                      \
        return { WORD, normalize(v.t), VERB, 0, flags };     \
    }                                                                   \
                                                                        \
    /* Try suffix-stripping using verb endings */                      \
    for (size_t ci = 0; ci < REG.size(); ++ci) {                       \
        for (size_t ei = 0; ei < REG[ci].endings.size(); ++ei) {       \
            const String& ending = REG[ci].endings[ei];           \
            int ending_form = REG[ci].form;                            \
            int ending_type = REG[ci].type;                            \
            /* NON-CONCATENATIVE TEMPLATIC DETECTION */                \
            if (ending_type == TRANSFIX) {           \
                for (const auto& dict_entry : DICTIONARY) {            \
                    String dict_pattern = dict_entry.root;        \
                    String template_pattern = ending;             \
                    WordString reconstructed = "";                    \
                    size_t dict_pos = 0;                               \
                    size_t temp_pos = 0;                               \
                    while (temp_pos < template_pattern.size() && dict_pos < dict_pattern.size()) { \
                        if (template_pattern[temp_pos] == '_') {       \
                            while (dict_pos < dict_pattern.size() && dict_pattern[dict_pos] != '_') { \
                                reconstructed += dict_pattern[dict_pos]; \
                                dict_pos++;                            \
                            }                                          \
                            if (dict_pos < dict_pattern.size() && dict_pattern[dict_pos] == '_') { \
                                dict_pos++;                            \
                            }                                          \
                            temp_pos++;                                \
                        } else {                                       \
                            while (dict_pos < dict_pattern.size() && dict_pattern[dict_pos] != '_') { \
                                reconstructed += dict_pattern[dict_pos]; \
                                dict_pos++;                            \
                            }                                          \
                            if (dict_pos < dict_pattern.size() && dict_pattern[dict_pos] == '_') { \
                                reconstructed += template_pattern[temp_pos]; \
                                dict_pos++;                            \
                            } else {                                   \
                                reconstructed += template_pattern[temp_pos]; \
                            }                                          \
                            temp_pos++;                                \
                        }                                              \
                    }                                                  \
                    while (temp_pos < template_pattern.size()) {       \
                        if (template_pattern[temp_pos] != '_') {       \
                            reconstructed += template_pattern[temp_pos]; \
                        }                                              \
                        temp_pos++;                                    \
                    }                                                  \
                    while (dict_pos < dict_pattern.size()) {           \
                        if (dict_pattern[dict_pos] != '_') {           \
                            reconstructed += dict_pattern[dict_pos];   \
                        }                                              \
                        dict_pos++;                                    \
                    }                                                  \
                    if (reconstructed == WORD) {                       \
                        Verb v = verb_lookup(DICTIONARY, dict_pattern.c_str()); \
                        if (v.t && *v.t) {         \
                            String translation = v.t;   \
                            const Morph* morph_to_ptr = &(MORPH_TO);   \
                            /* Create temporary MorphVariation for the template */ \
                            MorphVariation temp_var;                   \
                            temp_var.type = ending_type;               \
                            temp_var.ending = ending;                  \
                            temp_var.form = "";                        \
                            temp_var.flag = 0;                         \
                            temp_var.morphology = 0;                   \
                            temp_var.result_type = VERB;               \
                            temp_var.stackable = STACKABLE;            \
                            temp_var.vowel_harmony = nullptr;          \
                            temp_var.agreement = 0;                    \
                            translation = apply_morph(translation, &temp_var, morph_to_ptr, v.flags); \
                            return { WORD, normalize(translation), VERB, 0, v.flags }; \
                        }                                             \
                    }                                                 \
                }                                                     \
                continue;                                             \
            }                                                         \
            /* Regular suffix detection */                            \
            if (WORD.size() <= ending.size()) continue;               \
            if (WORD.compare(WORD.size() - ending.size(), ending.size(), ending) != 0) { \
                continue;                                             \
            }                                                         \
            String root = WORD.substr(0, WORD.size() - ending.size()); \
            Verb v = verb_lookup(DICTIONARY, root.c_str());           \
            if (v.t && *v.t) {                    \
                String translation = v.t;              \
                String affix = "";                               \
                String result = translation;                     \
                bool conjugation_applied = false;                     \
                for (const auto& conj : DEF) {                        \
                    String affix = conj.affix;                   \
                    if (conj.form == ending_form) {                   \
                        bool condition_met = conj.required_ending.empty() || \
                            (translation.size() >= conj.required_ending.size() && \
                             translation.compare(translation.size() - conj.required_ending.size(), \
                                                conj.required_ending.size(), \
                                                conj.required_ending) == 0); \
                        if (condition_met) {                          \
                            String stem = translation;           \
                            if (conj.vowel_harmony != nullptr) {      \
                                const HarmonyTable& v_h = *(conj.vowel_harmony); \
                                affix = checkVowelHarmony(stem, conj.affix, v_h); \
                            }                                         \
                            if (!conj.required_ending.empty()) {      \
                                stem = translation.substr(0, translation.size() - conj.required_ending.size()); \
                            }                                         \
                            if (conj.type == PREFIX) {                \
                                result = affix + translation;         \
                            } else if (conj.type == PREV_WORD) {      \
                                if (!affix.empty() && affix.back() != ' ') { \
                                    result = affix + " " + translation; \
                                }                                     \
                            } else if (conj.type == SUFFIX) {         \
                                result = translation + affix;         \
                            } else if (conj.type == NEXT_WORD) {      \
                                if (!affix.empty() && affix.front() != ' ') { \
                                    result = translation + " " + affix; \
                                }                                     \
                            }                                         \
                            translation = stem;                       \
                            conjugation_applied = true;               \
                            break;                                    \
                        }                                             \
                    }                                                 \
                }                                                     \
                if (conjugation_applied) {                            \
                    return { WORD, normalize(result), VERB, 0, v.flags }; \
                }                                                     \
            }                                                         \
        }                                                             \
    }                                                                 \
    /* If no verb conjugation matched, try morphological derivation */ \
    if (USE_MORPH) {                                                  \
        const Morph* morph_from_ptr = &(MORPH_FROM);                  \
        const Morph* morph_to_ptr = &(MORPH_TO);                      \
        String current_word = WORD;                              \
        Vector<const MorphVariation*> applied_morphs;            \
        bool found_root = false;                                      \
        const char* result = nullptr;                                 \
        uint64_t flags = 0;                                           \
        int safety = 10;                                              \
        while (!found_root && safety-- > 0) {                         \
            MorphResult p = detect_morph(current_word, morph_from_ptr); \
            if (p.matched_variation != nullptr) {                     \
                if (p.matched_variation->stackable == STACKABLE) {    \
                    applied_morphs.push_back(p.matched_variation);    \
                    current_word = p.root;                            \
                    Verb v = verb_lookup(DICTIONARY, current_word.c_str()); \
                    if (v.t && *v.t) {            \
                        result = v.t;                       \
                        flags = v.flags;                              \
                        found_root = true;                            \
                        break;                                        \
                    }                                                 \
                } else {                                              \
                    applied_morphs.push_back(p.matched_variation);    \
                    Verb v = verb_lookup(DICTIONARY, p.root.c_str()); \
                    if (v.t && *v.t) {            \
                        result = v.t;                       \
                        flags = v.flags;                              \
                        found_root = true;                            \
                    }                                                 \
                    break;                                            \
                }                                                     \
            } else {                                                  \
                break;                                                \
            }                                                         \
        }                                                             \
        if (found_root) {                                             \
            String translation = result;                         \
            for (auto it = applied_morphs.rbegin(); it != applied_morphs.rend(); ++it) { \
                translation = apply_morph(translation, *it, morph_to_ptr, flags); \
            }                                                         \
            return { WORD, normalize(translation), VERB, 0, flags };  \
        }                                                             \
    }                                                                 \
}

   #define SUFFIX_LOOKUP(DICTIONARY, WORD, ADJECTIVES)                         \
{                                                                           \
 String translation;\
 int word_type = 0;\
  for (int len = 6; len >= 2; --len) {\
    if (WORD.length() >= static_cast<size_t>(len)) {\
        AtomString ending = WORD.substr(WORD.length() - len);\
        Suffix suffResult = lookupSuff(DICTIONARY, ending.c_str());\
        if (suffResult.t) {\
            const char* mapped = suffResult.t;\
            AtomString stem = WORD.substr(0, WORD.length() - len);\
            word_type = suffResult.type;\
            const char* adjResult = lookup(ADJECTIVES, stem.c_str());\
            if (adjResult) {\
                translation = String(adjResult) + mapped;\
            word_type = suffResult.type;\
            }\
            else if (!stem.empty()) {\
                String altStem = stem.substr(0, stem.length() - 1) + "o";\
                const char* altAdj = lookup(ADJECTIVES, altStem.c_str());\
                if (altAdj)\
                    translation = String(altAdj) + mapped;\
                else\
                    translation = stem + mapped;\
            }\
            else {\
                translation = stem + mapped;\
            }\
            return Word{WORD, normalize(translation), word_type};\
        }\
    }\
}                                                                            \
}


   


enum NORMALIZATION_RULES {
   REPLACE_ALL = 0,
   REPLACE_START = 1,
   REPLACE_END = 2
};
#define NORMALIZE(ORIGINAL, RULE, REPLACEMENT)                     \
    do {                                                            \
        const String& orig = ORIGINAL;                         \
        const String& repl = REPLACEMENT;                      \
        size_t word_len = word.length();                            \
        size_t orig_len = orig.length();                             \
        if (word_len > orig_len) {                                   \
                             \
        if (RULE == REPLACE_ALL) {\
                size_t pos = 0;\
    while (pos + orig_len <= normalized_.length()) {\
        if (normalized_.compare(pos, orig_len, orig) == 0) {\
            normalized_.replace(pos, orig_len, repl);\
            pos += repl.length();\
        } else {\
            ++pos;\
        }\
    }\
}                                         \
                                                \
            if (RULE == REPLACE_START && normalized_.compare(0, orig_len, orig) == 0) { \
                normalized_ = repl + normalized_.substr(orig_len);    \
            }                                                        \
                                                \
            if (RULE == REPLACE_END && normalized_.length() >= orig_len && \
                normalized_.compare(normalized_.length() - orig_len, orig_len, orig) == 0) { \
                normalized_ = normalized_.substr(0, normalized_.length() - orig_len) + repl; \
            }                                                        \
        }                                                            \
    } while(0)


typedef struct {

    String prefix;
    String suffix;
} Circumfix;

typedef struct {
    String infix;
    int pos;
} Infix;

typedef struct {
    const char* w;
    const char* t;
    uint64_t orig_flags; // flags for original word, just remembered that in languages that have same linguistic features but with variation (e.g gender in portuguese and russian) we need to know the flags for both the original word and the translation to make decisions.
    uint64_t flags;
    const char* w2; // not elegant at all, but handles multi script, so we can translate between three different scripts to other three different, like [(bopomofo, hanzi, pinyin) -> (kana, kanji, romaji)]
    const char* w3;
    const char* t2;
    const char* t3;
    uint8_t word_type;
    const char* custom_ipa;
} Entry;

// to store phonetics and generate IPA transcriptions

enum Position {
   ALWAYS = 0,
   BEGINNING = 1,
   ENDING = 2,
   BEFORE = 3,
   AFTER = 4,
   BETWEEN = 5,
   ISOLATED = 6
}; 

enum StressPosition {
    STRESS_LAST = 0,
    STRESS_PENULT = 1,
    STRESS_ANTEPENULT = 2,
    STRESS_FIRST = 3,
    STRESS_SECOND = 4,
    STRESS_NONE = 5
};

typedef struct {
    StressPosition position;
    int fallback;  
} StressRule;

typedef struct {
    String string;
    String sound;
} IpaRule;
// e.g
//  a ALWAYS = ä, a BEGINNING = a
// a BEFORE b = e

typedef struct {
    IpaRule rule;
    int position;
    String with; 
} IpaRules;

// for each module you can write a list of ipa rules
// and i'll write a function that parses words and also allows IPA output
IpaRules ipa_rules[] = {
   { {"x", "tʃ"}, ALWAYS, ""},
   { {"x", "j"}, ENDING, ""},
   { {"e", "æ"}, ALWAYS, ""},
   { {"e", "æ"}, ALWAYS, ""},
   { {"u", "ɯ"}, ALWAYS, ""},
   { {"o", "oʔ"}, ENDING, ""},
   { {"g", "ŋ"}, ENDING, ""}
};


struct Verb {
    const char* w;       
    const char* t; 
    uint64_t orig_flags;
    uint64_t flags;
    uint64_t verb_flags;
    const char* w2; // not elegant at all, but handles multi script, so we can translate between three different scripts to other three different, like [(bopomofo, hanzi, pinyin) -> (kana, kanji, romaji)]
    const char* w3;
    const char* t2;
    const char* t3;
};


typedef struct
{
  const char* w;
  const char* t;
  int type;
  uint8_t flags;
} Suffix;


struct CaseVariation {
    uint8_t flag;
    uint8_t gender;
    String ending;
    String form;
    HarmonyTable* v_h;
    int affix_type; 
};

struct Case {
    int type; 
    Vector<CaseVariation> variations;
};

struct CaseResult {
    String root;
    const CaseVariation* matched_variation; 
};


enum Morphology {
   PLURAL_MORPH = 0,
   DIMINUTIVE_MORPH = 1,
   AUGMENTATIVE_MORPH = 2,
   COMPARATIVE_MORPH = 3,
   SUPERLATIVE_MORPH = 4,
   DEFINITE_MORPH = 5,
   AGENT_MORPH = 6,
   GENDER_MORPH = 7,
   AGENTIZE_DEVERB = 8,
   RECIPROCITY = 9,
   GENITIVE_MORPH = 10,
   LOCATIVE_MORPH = 11,
   FOCUS_MORPH = 12,
   ABESSIVE_MORPH = 13,
   COMITATIVE_MORPH = 14,
   PASSIVE_VOICE_MORPH = 15

};

enum SHOULD_STACK {
   STACKABLE = 0,
   NOT_STACKABLE = 1

};

enum AGREEMENT {
    NO_AGREEMENT = 0,
    AGREEMENT = 1

};




struct Morph {
    Vector<MorphVariation> variations;
};


struct MorphResult {
    String root;
    const MorphVariation* matched_variation; 
};



// Y is considered a vowel for english reasons obviously but one day i'll see what to do, but only
// if another language i implement needs a vowel as a consonant
inline bool isVowel(char x)
{
    if (x == 'a' || x == 'e' || x == 'i' || x == 'o'
        || x == 'u' || x == 'y' || x == 'A' || x == 'E' || x == 'I'
        || x == 'O' || x == 'U' || x == 'Y')
    return true;
    else
     return false;
}

inline bool isNumber(String x){
    if(x == "1" || x == "2" || x == "3" || x == "4" || x == "5" || x == "6" || x == "7" || x == "8" || x == "9"){
          return true;
    }
    return false;
}
struct Letter {
    int byte_pos;   
    String letter;  
};


Letter getFirstVowel(const String& word) {
    size_t byte_pos = 0;
    
    while (byte_pos < word.length()) {
        unsigned char c = static_cast<unsigned char>(word[byte_pos]);
        size_t char_len = 1;
        
        if ((c & 0x80) == 0) {
            char_len = 1;
        } else if ((c & 0xE0) == 0xC0) {
            char_len = 2;
        } else if ((c & 0xF0) == 0xE0) {
            char_len = 3;
        } else if ((c & 0xF8) == 0xF0) {
            char_len = 4;
        }
        
        String current_char = word.substr(byte_pos, char_len);
        
        bool is_vowel = false;
        if (char_len == 1) {
            char ch = word[byte_pos];
            is_vowel = (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u' ||
                       ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U');
        } else if (char_len == 2 && c == 0xD0) {
            unsigned char c2 = static_cast<unsigned char>(word[byte_pos + 1]);
            // Cyrillic vowels
            is_vowel = (c2 == 0xB0 || c2 == 0xB5 || c2 == 0xB8 || c2 == 0xBE || 
                       c2 == 0x83 || c2 == 0x8B || c2 == 0xAD || c2 == 0xAE || c2 == 0xAF ||
                       (c2 >= 0x90 && c2 <= 0x9F));
        }
        
        if (is_vowel) {
            return {static_cast<int>(byte_pos), current_char};
        }
        
        byte_pos += char_len;
    }
    
    return {-1, ""};
}

Letter getLastVowel(const String& word) {
    size_t byte_pos = 0;
    Letter last_vowel = {-1, ""};
    
    while (byte_pos < word.length()) {
        unsigned char c = static_cast<unsigned char>(word[byte_pos]);
        size_t char_len = 1;
        
        if ((c & 0x80) == 0) {
            char_len = 1;
        } else if ((c & 0xE0) == 0xC0) {
            char_len = 2;
        } else if ((c & 0xF0) == 0xE0) {
            char_len = 3;
        } else if ((c & 0xF8) == 0xF0) {
            char_len = 4;
        }
        
        String current_char = word.substr(byte_pos, char_len);
        
        bool is_vowel = false;
        if (char_len == 1) {
            char ch = word[byte_pos];
            is_vowel = (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u' ||
                       ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U');
        } else if (char_len == 2 && c == 0xD0) {
            unsigned char c2 = static_cast<unsigned char>(word[byte_pos + 1]);
            is_vowel = (c2 == 0xB0 || c2 == 0xB5 || c2 == 0xB8 || c2 == 0xBE || 
                       c2 == 0x83 || c2 == 0x8B || c2 == 0xAD || c2 == 0xAE || c2 == 0xAF ||
                       (c2 >= 0x90 && c2 <= 0x9F));
        }
        
        if (is_vowel) {
            last_vowel = {static_cast<int>(byte_pos), current_char};
        }
        
        byte_pos += char_len;
    }
    
    return last_vowel;
}

// Helper to convert character position to byte position
size_t charPosToBytePos(const String& str, int char_pos) {
    if (char_pos <= 0) return 0;
    
    size_t byte_pos = 0;
    int chars_found = 0;
    
    while (byte_pos < str.length() && chars_found < char_pos) {
        unsigned char c = static_cast<unsigned char>(str[byte_pos]);
        if ((c & 0x80) == 0) {
            byte_pos += 1;
        } else if ((c & 0xE0) == 0xC0) {
            byte_pos += 2;
        } else if ((c & 0xF0) == 0xE0) {
            byte_pos += 3;
        } else {
            byte_pos += 4;
        }
        chars_found++;
    }
    
    return byte_pos;
}

// if the lookup passes a vowel harmony table
// look for the corresponding vowel for the dominant one we found
// apply to the affix
// return it
inline String checkVowelHarmony(const String& word, String affix, const HarmonyTable& table){
    Letter last_vowel = getLastVowel(word);
    if(last_vowel.letter.empty()) return affix; 
    
    for(const Harmony& rule : table){
        // Manual find - check if last_vowel exists in rule.vowels
        bool found = false;
        for(const auto& vowel : rule.vowels) {
            if(vowel == last_vowel.letter) {
                found = true;
                break;
            }
        }
        
        if(found){
            size_t pos = affix.find('_');
            if(pos != String::npos){
                affix.replace(pos, 1, rule.insert_vowel);
            }
            break;
        }
    }
    
    return affix;
}

inline CaseResult detect_case(const String& word, const Case* case_from) {
    if (!case_from) {
        return { word, nullptr };
    }

    for (const auto& var : case_from->variations) {
        const String& affix = var.form;

        if (var.affix_type == SUFFIX) {
            if (word.size() >= affix.size() &&
                word.compare(word.size() - affix.size(), affix.size(), affix) == 0)
            {
                String root = word.substr(0, word.size() - affix.size());
                return { root, &var };
            }
        } else if (var.affix_type == PREFIX) {
            if (word.size() >= affix.size() &&
                word.compare(0, affix.size(), affix) == 0)
            {
                String root = word.substr(affix.size());
                return { root, &var };
            }
        }
        // Add other affix types as needed
    }
    return { word, nullptr };
}

inline String apply_case(
    const String& translation,
    const CaseVariation* from_var,
    const Case* case_to,
    uint64_t flags,
    uint8_t target_case_type)
{
    if (!case_to || case_to->variations.empty())
        return translation;
        
if (!from_var && case_to) {
    for (const auto& var : case_to->variations) {

        if (var.flag == target_case_type) {
            if (flags == 0 || (var.gender & flags)) {
                String result = translation;
                String affix = "";
                if (var.v_h != nullptr) {
                    affix = checkVowelHarmony(translation, var.form, *(var.v_h));
                } else {
                    affix = var.form;
                }
                
                if (!affix.empty() && translation.size() >= affix.size() &&
                    translation.compare(translation.size() - affix.size(), affix.size(), affix) == 0) {
                   return translation;
                }
                
                if (var.affix_type == SUFFIX) {
                    result += affix;
                } else if (var.affix_type == PREFIX) {
                    result = affix + result;
                }
               return result;
            }
        }
    }
    return translation;
}else if (from_var && !case_to) {
        // Case → non-case
        const String& affix = from_var->form;
        if (from_var->affix_type == SUFFIX) {
            if (translation.size() >= affix.size() &&
                translation.compare(translation.size() - affix.size(), affix.size(), affix) == 0)
            {
                return translation.substr(0, translation.size() - affix.size());
            }
        } else if (from_var->affix_type == PREFIX) {
            if (translation.size() >= affix.size() &&
                translation.compare(0, affix.size(), affix) == 0)
            {
                return translation.substr(affix.size());
            }
        }
        return translation;
    } else if (from_var && case_to) {
        // Case → case
        for (const auto& var : case_to->variations) {
            if (var.flag == from_var->flag) {
                if (flags == 0 || (var.gender & flags)) {
                    const String& ending = var.ending;
                    const String& form = var.form;
                    String affix = "";
                    if (var.v_h != nullptr) {
                        affix = checkVowelHarmony(translation, form, *(var.v_h));
                    } else {
                        affix = form;
                    }
                    
                    if (translation.size() >= ending.size() &&
                        translation.compare(translation.size() - ending.size(), ending.size(), ending) == 0)
                    {
                        String result = translation.substr(0, translation.size() - ending.size());
                        if (var.affix_type == SUFFIX) {
                            result += affix;
                        } else if (var.affix_type == PREFIX) {
                            result = affix + result;
                        }
                        return result;
                    }
                }
            }
        }
        return translation;
    }

    return translation;
}

inline MorphResult detect_morph(const String& word, const Morph* morph_from, const Vector<String> sentence, int word_index) {
    if (!morph_from) {
        return { word, nullptr };
    }
    if (word.find(PREV_MARKER) != String::npos) {
        return { word, nullptr };
    }

    if (word_index < 0 || word_index >= (int)sentence.size()) {
        return { word, nullptr };
    }
    for (const auto& var : morph_from->variations) {
        int type = var.type;  
        

        switch(type){
       
        case PREV_WORD: {
    const String& prev_word = var.ending; 
    
if (word_index > 0 && word_index < sentence.size() && sentence[word_index - 1] == prev_word){
        String marked = word + PREV_MARKER;
         return { marked, &var };
    }
    break;
}
        case NEXT_WORD:
        {
                const String& next_word = var.ending; 
                const String& singular_ending = var.form;  
                // i need to think about this, now this part does have access to the other itens on the array, but i'm not sure how to modify for removal
                if (word_index < sentence.size() && sentence[word_index + 1] == next_word)
                {
                    return { word, &var };
                }

        }
        break;
      case TOTAL_REDUPLICATION:
        {
    const String& trigger = var.form;
    int times = string_to_int(trigger);
    int word_len = word.length();
    
    if (times == 2) {
        // is word length ian even number
        if (word_len % 2 == 0) {
            int half_len = word_len / 2;
            String first_half = word.substr(0, half_len);
            String second_half = word.substr(half_len);
            
            if (first_half == second_half) {
                return { first_half, &var };
            }
        }
    }
    else if (times == 3) {
        // Check if word length is divisible by 3
        if (word_len % 3 == 0) {
            int third_len = word_len / 3;
            String first_third = word.substr(0, third_len);
            String second_third = word.substr(third_len, third_len);
            String third_third = word.substr(third_len * 2);
            
            if (first_third == second_third && first_third == third_third) {
                return { first_third, &var };
            }
        }
    }
}
// i'm not sure if natural languages out there use quadruple reduplication, i'll stop there for now 
break;
      case PARTIAL_REDUPLICATION_PREFIX:
        {
            String pattern = var.ending; // like "CVC", "CV", "VC", etc.
            String trigger = var.form;   // "B" for beginning, "E" for ending
            String result = word;
            
            if (trigger == "B") {
                // Check if word is long enough to match pattern
                if (word.length() >= pattern.length()) {
                    bool matches = true;
                    
                    // Check each character position against pattern
                    for (int i = 0; i < pattern.length(); i++) {
                        char patternChar = pattern[i];
                        char wordChar = word[i];
                        
                        if (patternChar == 'C') {
                            // Expecting a consonant
                            if (isVowel(wordChar)) {
                                matches = false;
                                break;
                            }
                        } 
                        else if (patternChar == 'V') {
                            // Expecting a vowel
                            if (!isVowel(wordChar)) {
                                matches = false;
                                break;
                            }
                        }
                    }
                    
                    if (matches) {
                        result = word.substr(pattern.length());
                        return { result, &var };
                    }
                }
            }
            else if (trigger == "E") {
                if (word.length() >= pattern.length()) {
                    bool matches = true;
                    int startPos = word.length() - pattern.length();
                    
                    for (int i = 0; i < pattern.length(); i++) {
                        char patternChar = pattern[i];
                        char wordChar = word[startPos + i];
                        
                        if (patternChar == 'C') {
                            if (isVowel(wordChar)) {
                                matches = false;
                                break;
                            }
                        } 
                        else if (patternChar == 'V') {
                            if (!isVowel(wordChar)) {
                                matches = false;
                                break;
                            }
                        }
                    }
                    
                    if (matches) {
        // return if found at correct position (here it ssufix)
                        result = word.substr(0, startPos);
                        return { result, &var };
                    }
                }
            }
        
            return { word, &var };
        }
break;
case PARTIAL_REDUPLICATION_SUFFIX:  
    {
        String pattern = var.ending;
        String trigger = var.form;
        String result = word;
        String matchedPart;
        
        // look for pattern at begninning
        if (trigger == "B") {
            if (word.length() >= pattern.length()) {
                bool matches = true;
                for (int i = 0; i < pattern.length(); i++) {
                    char patternChar = pattern[i];
                    char wordChar = word[i];
                    // match 'C' or 'V' with the current char
                    if (patternChar == 'C' && isVowel(wordChar)) {
                        matches = false;
                        break;
                    }
                    else if (patternChar == 'V' && !isVowel(wordChar)) {
                        matches = false;
                        break;
                    }
                }
                if (matches) {
                    matchedPart = word.substr(0, pattern.length());
                }
            }
        }
          // look for pattern at ending
        else if (trigger == "E") {
            if (word.length() >= pattern.length()) {
                bool matches = true;
                int startPos = word.length() - pattern.length();
                for (int i = 0; i < pattern.length(); i++) {
                    char patternChar = pattern[i];
                    char wordChar = word[startPos + i];
                    
                    if (patternChar == 'C' && isVowel(wordChar)) {
                        matches = false;
                        break;
                    }
                    else if (patternChar == 'V' && !isVowel(wordChar)) {
                        matches = false;
                        break;
                    }
                }
                if (matches) {
                    matchedPart = word.substr(startPos);
                }
            }
        }
        // return if found at correct position (here it ssufix)
        if (!matchedPart.empty() && word.length() >= matchedPart.length()) {
            if (word.substr(word.length() - matchedPart.length()) == matchedPart) {
                result = word.substr(0, word.length() - matchedPart.length());
                return { result, &var };
            }
        }
        
        return { word, &var };
    }
       case TRANSFIX:
{
    // THE PIPELINE FOR TRANSFIXES IS ALL INSIDE OF LOOKUP AS OF RIGHT NOW :/
    return { word, &var };
}
break;
        case CIRCUMFIX:
          {  
            const String& full_circumfix = var.ending; 
        String delimiter = "$"; //common delimiter i use is '$'
        //get the prefix
        String pre = full_circumfix.substr(0, full_circumfix.find(delimiter));
               //get the suffix
         String suf = full_circumfix.substr((full_circumfix.find(delimiter) + 1), full_circumfix.length());
    
         // if the word length can afford (?) to have this circumfix
        if (word.size() >= full_circumfix.size()
        // does it start with the prefix?
       && word.substr(0, pre.length()) == pre
       // does it end wiht the suffix?
       && word.substr(word.length() - suf.length()) == suf)
        {
            String root = word.substr(pre.length(), word.length());
        // std::cout << "\nhere is the first part: " << pre;
        // std::cout << "\nhere is the last part: " << suf;
        return {root.substr(0, root.length() - suf.length()), &var};
        }
    }
        break;

         case SUFFIX:
        {
        const String& plural_ending = var.ending; 
        const String& singular_ending = var.form;  
        if (word.size() >= plural_ending.size() &&
            word.compare(word.size() - plural_ending.size(), plural_ending.size(), plural_ending) == 0)
        {
            String base = word.substr(0, word.size() - plural_ending.size());
            String root = base + singular_ending;
            return { root, &var };
        }}
        break;

        case PREFIX:
      {   const String& prefix = var.ending; 
         if (word.size() > prefix.size()
             && word.substr(0, prefix.length()) == prefix
        ){
             String root = word.substr(prefix.length(), word.length());
             return { root, &var };
         }}
        break;
        case SIMULFIX:
      {   
    const String& replacement = var.ending;
    const String& trigger = var.form;        


         
        if (word.size() > replacement.size()){
            String root = word;
            size_t pos = root.find(replacement); 
            if(pos != String::npos){
                root.replace(pos, replacement.size(), trigger);  
            } 
            return { root, &var };
        }}
        break;
case INFIX:
{
    const String& infix_to_remove = var.ending;  
    const String& trigger = var.form;            
    
    if (trigger == "BFV") {
        Letter first_vowel = getFirstVowel(word); 
        
        if (first_vowel.byte_pos >= 0) {
            int infix_pos = first_vowel.byte_pos - infix_to_remove.length();
            if (infix_pos >= 0 && 
                infix_pos + infix_to_remove.length() <= word.length() &&
                word.substr(infix_pos, infix_to_remove.length()) == infix_to_remove) {
                
                String root = word.substr(0, infix_pos) + 
                             word.substr(infix_pos + infix_to_remove.length());
                return { root, &var };
            } 
        }
    }
}
break;
    }
    }
    
    return { word, nullptr };
}

inline String apply_morph(
    const String& translation,
    const MorphVariation* from_var,
    const Morph* morph_to,
    uint8_t flag)
{
    
    if (!morph_to || !from_var) {
     
        return translation;
    }

    int source_morph = from_var->morphology;

    String result;
    for (const auto& var : morph_to->variations) {
    
        if (var.morphology == source_morph) {
           const String& ending = var.ending;
            const String& form   = var.form;
            
            String affix = ending;
            if (var.vowel_harmony != nullptr) {
                uintptr_t addr = reinterpret_cast<uintptr_t>(var.vowel_harmony);
                if (addr >= 0x1000 && addr < 0x7fffffffffff && 
                    !(addr >= 0x7fff00000000 && addr <= 0x7fffffffffff)) {
                    const HarmonyTable& v_h = *(var.vowel_harmony);
                    if (!v_h.empty()) {
                        affix = checkVowelHarmony(translation, ending, v_h);
                    } else {
                        affix = ending;
                    }
                } else {
                    affix = ending;
                }
            } else {
                affix = ending;
            }
            
            // DUPLICATE CHECK DISABLED - was preventing 'ax' from being added
            // if (!affix.empty() && translation.size() >= affix.size() &&
            //     translation.compare(translation.size() - affix.size(), affix.size(), affix) == 0) {
            //     return translation;
            // }

            // infixes and reduplication use a specific format for trigger so i'll do it apart
            if (var.type == TOTAL_REDUPLICATION) {
                const String& trigger = var.form;
                int times = string_to_int(trigger);
                String result = translation;
                switch (times) {
                    case 2: result = translation + translation; break;
                    case 3: result = translation + translation + translation; break;
                    default: break;
                }
                  return result;
            }
            if (var.type == PARTIAL_REDUPLICATION_PREFIX) {
                String pattern = var.ending;
                String trigger = var.form;
                String result = translation;
                String matchedPart;
                
                if (trigger == "B") {
                    if (translation.length() >= pattern.length()) {
                        bool matches = true;
                        for (int i = 0; i < pattern.length(); i++) {
                            char patternChar = pattern[i];
                            char wordChar = translation[i];
                            if (patternChar == 'C' && isVowel(wordChar)) {
                                matches = false;
                                break;
                            } else if (patternChar == 'V' && !isVowel(wordChar)) {
                                matches = false;
                                break;
                            }
                        }
                        if (matches) {
                            matchedPart = translation.substr(0, pattern.length());
                        }
                    }
                } else if (trigger == "E") {
                    if (translation.length() >= pattern.length()) {
                        bool matches = true;
                        int startPos = translation.length() - pattern.length();
                        for (int i = 0; i < pattern.length(); i++) {
                            char patternChar = pattern[i];
                            char wordChar = translation[startPos + i];
                            if (patternChar == 'C' && isVowel(wordChar)) {
                                matches = false;
                                break;
                            } else if (patternChar == 'V' && !isVowel(wordChar)) {
                                matches = false;
                                break;
                            }
                        }
                        if (matches) {
                            matchedPart = translation.substr(startPos);
                        }
                    }
                }
                
                if (!matchedPart.empty()) {
                    result = matchedPart + translation;
                }
               return result;
            }
            if (var.type == PARTIAL_REDUPLICATION_SUFFIX) {
                String pattern = var.ending;
                String trigger = var.form;
                String result = translation;
                String matchedPart;
                
                if (trigger == "B") {
                    if (translation.length() >= pattern.length()) {
                        bool matches = true;
                        for (int i = 0; i < pattern.length(); i++) {
                            char patternChar = pattern[i];
                            char wordChar = translation[i];
                            if (patternChar == 'C' && isVowel(wordChar)) {
                                matches = false;
                                break;
                            } else if (patternChar == 'V' && !isVowel(wordChar)) {
                                matches = false;
                                break;
                            }
                        }
                        if (matches) {
                            matchedPart = translation.substr(0, pattern.length());
                        }
                    }
                } else if (trigger == "E") {
                    if (translation.length() >= pattern.length()) {
                        bool matches = true;
                        int startPos = translation.length() - pattern.length();
                        for (int i = 0; i < pattern.length(); i++) {
                            char patternChar = pattern[i];
                            char wordChar = translation[startPos + i];
                            if (patternChar == 'C' && isVowel(wordChar)) {
                                matches = false;
                                break;
                            } else if (patternChar == 'V' && !isVowel(wordChar)) {
                                matches = false;
                                break;
                            }
                        }
                        if (matches) {
                            matchedPart = translation.substr(startPos);
                        }
                    }
                }
                
                if (!matchedPart.empty()) {
                    result = translation + matchedPart;
                }
                return result;
            }
            if (var.type == INFIX) {
                if (var.required_flags == 0 || (flag & var.required_flags) == var.required_flags) {
                    const String& infix_to_add = var.ending;
                    const String& trigger = var.form;
                    String result = translation;
                    
                    auto charPosToBytePos = [](const String& str, int char_pos) -> size_t {
                        if (char_pos <= 0) return 0;
                        size_t byte_pos = 0;
                        int chars_found = 0;
                        while (byte_pos < str.length() && chars_found < char_pos) {
                            unsigned char c = static_cast<unsigned char>(str[byte_pos]);
                            if ((c & 0x80) == 0) byte_pos += 1;
                            else if ((c & 0xE0) == 0xC0) byte_pos += 2;
                            else if ((c & 0xF0) == 0xE0) byte_pos += 3;
                            else byte_pos += 4;
                            chars_found++;
                        }
                        return byte_pos;
                    };
                    
                    bool is_number = !trigger.empty();
                    for (char c : trigger) {
                        if (c < '0' || c > '9') {
                            is_number = false;
                            break;
                        }
                    }
                    
                    if (is_number) {
                        int char_pos = string_to_int(trigger);
                        size_t byte_pos = charPosToBytePos(result, char_pos);
                        if (byte_pos <= result.length()) {
                            result.insert(byte_pos, infix_to_add);
                        }
                    } else if (trigger == "BFV") {
                        Letter first_vowel = getFirstVowel(result);
                        if (first_vowel.byte_pos >= 0) {
                            result.insert(first_vowel.byte_pos, infix_to_add);
                        }
                    } else if (trigger == "BLV") {
                        Letter last_vowel = getLastVowel(result);
                        if (last_vowel.byte_pos >= 0) {
                            result.insert(last_vowel.byte_pos, infix_to_add);
                        }
                    }
                    return result;
                }
            }
            if (var.type == SIMULFIX) {
                const String& replacement = var.ending;
                const String& trigger = var.form;
                String result = translation;
                size_t pos = result.find(trigger);
                if (pos != String::npos) {
                    result.replace(pos, trigger.size(), replacement);
                }
                return result;
            }
          else if (form.empty() || 
                (translation.size() >= form.size() &&
                translation.compare(translation.size() - form.size(), form.size(), form) == 0)) {
                switch (var.type) {
                    case SUFFIX:
                        result = translation.substr(0, translation.size() - form.size());
                        result += affix;
                        return result;
                    case PREFIX:
                        result = affix + translation.substr(form.size());
                        return result;
                    case PREV_WORD:
                        result += form;
                        result += " ";
                        return translation;
                    case NEXT_WORD:
                        result += translation;
                        result += affix;
                        return result;
                    case TRANSFIX: {
                        String dict_pattern = translation;
                        String template_pattern = var.ending;
                        String result = "";
                        size_t dict_pos = 0;
                        size_t temp_pos = 0;
                        while (temp_pos < template_pattern.size() && dict_pos < dict_pattern.size()) {
                            if (template_pattern[temp_pos] == '_') {
                                while (dict_pos < dict_pattern.size() && dict_pattern[dict_pos] != '_') {
                                    result += dict_pattern[dict_pos];
                                    dict_pos++;
                                }
                                if (dict_pos < dict_pattern.size() && dict_pattern[dict_pos] == '_') {
                                    dict_pos++;
                                }
                                temp_pos++;
                            } else {
                                while (dict_pos < dict_pattern.size() && dict_pattern[dict_pos] != '_') {
                                    result += dict_pattern[dict_pos];
                                    dict_pos++;
                                }
                                if (dict_pos < dict_pattern.size() && dict_pattern[dict_pos] == '_') {
                                    result += template_pattern[temp_pos];
                                    dict_pos++;
                                } else {
                                    result += template_pattern[temp_pos];
                                }
                                temp_pos++;
                            }
                        }
                        while (temp_pos < template_pattern.size()) {
                            if (template_pattern[temp_pos] != '_') {
                                result += template_pattern[temp_pos];
                            }
                            temp_pos++;
                        }
                        while (dict_pos < dict_pattern.size()) {
                            if (dict_pattern[dict_pos] != '_') {
                                result += dict_pattern[dict_pos];
                            }
                            dict_pos++;
                        }
                        if (var.flag != 0) {
                            flag |= var.flag;
                        }
                        return result;
                    }
                    case CIRCUMFIX: {
                        const String& full_circumfix = affix;
                        String delimiter = "$";
                        String pre = full_circumfix.substr(0, full_circumfix.find(delimiter));
                        String suf = full_circumfix.substr((full_circumfix.find(delimiter) + 1), full_circumfix.length());
                        return pre + translation + suf;
                    }
                }
            }
        }
    }
    return translation;
}
using Dictionary = Entry[];
using VerbDictionary = Verb[];
using SuffixDictionary = Suffix[];



#define ENTRY(w, t) { w, t, 0 }

#define SYNONYMS_2(t, a, b) \
    ENTRY(a, t), ENTRY(b, t)

#define SYNONYMS_3(t, a, b, c) \
    ENTRY(a, t), ENTRY(b, t), ENTRY(c, t)

#define SYNONYMS_4(t, a, b, c, d) \
    ENTRY(a, t), ENTRY(b, t), ENTRY(c, t), ENTRY(d, t)


#define GET_MACRO(_1,_2,_3,_4,NAME,...) NAME

#define SYNONYMS(t, ...) \
    GET_MACRO(__VA_ARGS__, \
              SYNONYMS_4, \
              SYNONYMS_3, \
              SYNONYMS_2)(t, __VA_ARGS__)

#define DICT(name, ...) constexpr Dictionary name = __VA_ARGS__
#define V_DICT(name, ...) constexpr VerbDictionary name = __VA_ARGS__
#define SUFFIX_RULES(name, ...) constexpr SuffixDictionary name = __VA_ARGS__
#define LIST(name, ...) Vector<SuffixRule> name = __VA_ARGS__

#define MORPH_DEF(name,...) inline Morph name = {__VA_ARGS__}
#define CASE_DEF(name,...) inline Case name = {0, {__VA_ARGS__}}


#define VERB_ENDINGS(name, ...) inline VerbRuleDictionary name = __VA_ARGS__

#define VERB_CONJUGATIONS(name, ...) inline VerbConjugationDictionary  name = __VA_ARGS__




#define COMBINE_VERB_TOKENS(INPUT, OUTPUT, VERB_DICT, ENDINGS, MAX_STEM, MAX_ENDING) \
    do { \
        Vector<String> temp; \
        size_t i = 0; \
        while (i < (INPUT).size()) { \
            bool found = false; \
            for (size_t stem_len = (MAX_STEM); stem_len >= 1; --stem_len) { \
                if (i + stem_len > (INPUT).size()) continue; \
                String stem_candidate; \
                for (size_t k = 0; k < stem_len; ++k) \
                    stem_candidate += (INPUT)[i + k]; \
                Verb v = verb_lookup((VERB_DICT), stem_candidate.c_str()); \
                if (v.t && *v.t) { \
                    for (size_t ending_len = (MAX_ENDING); ending_len >= 1; --ending_len) { \
                        if (i + stem_len + ending_len > (INPUT).size()) continue; \
                        String ending_candidate; \
                        for (size_t k = 0; k < ending_len; ++k) \
                            ending_candidate += (INPUT)[i + stem_len + k]; \
                        bool ending_match = false; \
                        for (const auto& group : (ENDINGS)) { \
                            for (const auto& e : group.endings) { \
                                if (ending_candidate == e) { \
                                    ending_match = true; \
                                    break; \
                                } \
                            } \
                            if (ending_match) break; \
                        } \
                        if (ending_match) { \
                            temp.push_back(stem_candidate + ending_candidate); \
                            i += stem_len + ending_len; \
                            found = true; \
                            break; \
                        } \
                    } \
                    if (found) break; \
                } \
            } \
            if (!found) { \
                temp.push_back((INPUT)[i]); \
                ++i; \
            } \
        } \
        (OUTPUT) = temp; \
    } while(0)




#define DEFAULT_COMBINE_VERB_TOKENS(INPUT, OUTPUT, VERB_DICT, ENDINGS, MAX_STEM, MAX_ENDING) \
    do { \
        Vector<String> temp; \
        size_t i = 0; \
        while (i < (INPUT).size()) { \
            bool found = false; \
            for (size_t stem_len = (MAX_STEM); stem_len >= 1; --stem_len) { \
                if (i + stem_len > (INPUT).size()) continue; \
                String stem_candidate; \
                for (size_t k = 0; k < stem_len; ++k) \
                    stem_candidate += (INPUT)[i + k]; \
                Verb v = find_verb_in_array((VERB_DICT), verbs_length, stem_candidate.c_str());\
                if (v.t && *v.t) { \
                    for (size_t ending_len = (MAX_ENDING); ending_len >= 1; --ending_len) { \
                        if (i + stem_len + ending_len > (INPUT).size()) continue; \
                        String ending_candidate; \
                        for (size_t k = 0; k < ending_len; ++k) \
                            ending_candidate += (INPUT)[i + stem_len + k]; \
                        bool ending_match = false; \
                        for (const auto& group : (ENDINGS)) { \
                            for (const auto& e : group.endings) { \
                                if (ending_candidate == e) { \
                                    ending_match = true; \
                                    break; \
                                } \
                            } \
                            if (ending_match) break; \
                        } \
                        if (ending_match) { \
                         String combined;\
                            combined += stem_candidate;\
                            combined += ending_candidate;\
                            temp.push_back(combined);\
                            i += stem_len + ending_len; \
                            found = true; \
                            break; \
                        } \
                    } \
                    if (found) break; \
                } \
            } \
            if (!found) { \
                temp.push_back((INPUT)[i]); \
                ++i; \
            } \
        } \
        (OUTPUT) = temp; \
    } while(0)


#define COMBINE_VERB_STEMS(INPUT, OUTPUT, VERB_DICT, ENDINGS)              \
    do {                                                                    \
        Vector<Word> temp;                                             \
        for (size_t i = 0; i < (INPUT).size(); ++i) {                      \
            bool combined = false;                                          \
            Verb v = verb_lookup(VERB_DICT, (INPUT)[i].word.c_str());      \
            if (v.t && *v.t) {                         \
               if (i + 1 < (INPUT).size()) {                              \
                    const String& next = (INPUT)[i+1].word;           \
                    for (size_t ci = 0; ci < (ENDINGS).size(); ++ci) {     \
                        for (size_t ei = 0; ei < (ENDINGS)[ci].endings.size(); ++ei) { \
                            if (next == (ENDINGS)[ci].endings[ei]) {       \
                                 Word w;                                     \
                                w.word = (INPUT)[i].word + next;           \
                                w.translation = "";                        \
                                w.type = UNKNOWN;                          \
                                temp.push_back(w);                         \
                                i++; /* skip the ending token */           \
                                combined = true;                           \
                                break;                                      \
                            }                                               \
                        }                                                   \
                        if (combined) break;                               \
                    }                                                       \
                }                                                           \
            }                                                               \
            if (!combined) {                                                \
                temp.push_back((INPUT)[i]);                                \
            }                                                               \
        }                                                                   \
        (OUTPUT) = temp;                                                    \
    } while(0)



#define MAIN(name, NGRAMS, REORDER_HELPERS, LOOKUP_FUNCTION, MULTIBYTE, SPACED, VERB_ENDINGS, DICT_CHECK) \
String name(const char* sentence) { \
    char buffer[250]; \
    strncpy(buffer, sentence, sizeof(buffer)); \
    buffer[sizeof(buffer) - 1] = '\0'; \
    to_lower(buffer); \
    Vector<String> arr; \
    if(MULTIBYTE == true) { \
        arr = tokenize_cjk(String(buffer)); \
        /* 1. Combine verb stems + endings */ \
        Vector<String> verb_combined; \
        COMBINE_VERB_TOKENS(arr, verb_combined, verbs, VERB_ENDINGS, 3, 5); \
        Vector<String> final_combined; \
        size_t i = 0; \
        while (i < verb_combined.size()) { \
            bool found = false; \
            for (size_t len = 4; len >= 1; --len) { \
                if (i + len > verb_combined.size()) continue; \
                String cand; \
                for (size_t k = 0; k < len; ++k) cand += verb_combined[i + k]; \
                if (DICT_CHECK) { \
                    final_combined.push_back(cand); \
                    i += len; \
                    found = true; \
                    break; \
                } \
            } \
            if (!found) { \
                final_combined.push_back(verb_combined[i]); \
                ++i; \
            } \
        } \
        arr = final_combined; \
    } else { \
        arr = tokenize(String(buffer)); \
    } \
    String translated = trigramLookup(NGRAMS, arr, REORDER_HELPERS, LOOKUP_FUNCTION, SPACED); \
    return translated; \
}

#define DEFAULT()\
    reordered_arr.push_back(Word{ sentence_arr.at(i).word, normalize(sentence_arr.at(i).translation), sentence_arr.at(i).type,    sentence_arr.at(i).orig_flags, sentence_arr.at(i).flags, sentence_arr.at(i).trigger_word});\

#define HANDLE_CASE(INFO_ARG, FROM_CASE, TO_CASE) \
    if (!reordered_arr.empty()) { \
        for (size_t i = 0; i + 1 < reordered_arr.size(); ++i) { \
            auto &current = reordered_arr.at(i); \
            auto &next = reordered_arr.at(i + 1); \
            if (i > 0) { \
                auto &previous = reordered_arr.at(i - 1); \
                if (previous.trigger_word.empty() && \
                    current.type == VERB && (previous.type == NOUN || previous.type == PRONOUN)) { \
                    uint64_t f = lookupFlags_test(default_nouns, nouns_length, previous.word.c_str()); \
                    CaseResult g = detect_case(current.word, FROM_CASE); \
                    previous.translation = apply_case(previous.translation, nullptr, TO_CASE, f, NOMINATIVE); \
                } \
            } \
            if (current.type == VERB && (next.type == NOUN || next.type == PRONOUN)) { \
                uint64_t f = lookupFlags_test(default_nouns, nouns_length, next.word.c_str()); \
                CaseResult g = detect_case(current.word, FROM_CASE); \
                next.translation = apply_case(next.translation, nullptr, TO_CASE, f, ACCUSATIVE); \
            } \
        } \
        if ((INFO_ARG)->clause_order_to == SOV) { \
            for (size_t i = 0; i < reordered_arr.size(); ++i) { \
                if (reordered_arr[i].type == VERB) { \
                    size_t last_noun_idx = i + 1; \
                    for (size_t j = i + 1; j < reordered_arr.size(); ++j) { \
                        if (reordered_arr[j].type == NOUN || reordered_arr[j].type == PRONOUN) { \
                            last_noun_idx = j; \
                        } \
                    } \
                    if (last_noun_idx > i && last_noun_idx < reordered_arr.size()) { \
                        Word temp = reordered_arr[i]; \
                        reordered_arr[i] = reordered_arr[last_noun_idx]; \
                        reordered_arr[last_noun_idx] = temp; \
                    } \
                    break; \
                } \
            } \
        } \
    }
// have to rename this to general abstractions, sinc it doesnt handle only posession, but also adjective order, definiteness and futurally more
#define HANDLE_TRANSFERS(INFO_ARG, ARR, MORPH_TO_OBJ) \
    do { \
       if ((ARR).size() >= 1) { \
            for (int p_i = 0; p_i < (INFO_ARG)->def_from_count; ++p_i) { \
                Definiteness def_from = (INFO_ARG)->def_from[p_i]; \
                int type_from = def_from.type; \
                if(type_from == PREFIX || type_from == SUFFIX || type_from == CIRCUMFIX || type_from == INFIX) { \
                    for (size_t i = 0; i < (ARR).size(); ++i) { \
                        if (type_from == SUFFIX) { \
                            if(endsWith((ARR)[i].word, def_from.addition)) { \
                            } \
                        } \
                        if (type_from == PREFIX) { \
                            if((ARR)[i].word.substr(0, def_from.addition.length()) == def_from.addition) { \
                            } \
                        } \
                    } \
             } else { \
    for (size_t i = 1; i < (ARR).size(); ++i) { \
        if (type_from == PREV_WORD) { \
                    if ((ARR)[i - 1].word == def_from.addition) { \
                        for (int t_i = 0; t_i < (INFO_ARG)->def_to_count; ++t_i) { \
                            Definiteness def_to = (INFO_ARG)->def_to[t_i]; \
                              String affix = def_to.addition;\
                    if(def_to.vowel_harmony != nullptr){\
                                            const HarmonyTable& v_h = *(def_to.vowel_harmony); \
                                            affix = checkVowelHarmony((ARR)[i].translation, affix, v_h);\
                                  }\
                            if ((def_from.flags == 0 && def_to.flags == 0) || \
                                (def_from.flags != 0 && (def_to.flags & def_from.flags))) { \
                                 switch (def_to.type){\
                                    case PREV_WORD:\
                                    (ARR)[i - 1].translation = affix + " " + (ARR)[i].translation; \
                                    break;\
                                       case SUFFIX:\
                                    (ARR)[i - 1].translation = (ARR)[i].translation + affix; \
                                    break;\
                                 }\
                                 (ARR).erase((ARR).begin() + i - 1); \
                            } \
                        } \
                    } \
                } \
            } \
        } \
            } \
        } \
   if ((ARR).size() >= 2) { \
            Vector<Word> replacement; \
            AdjectiveOrder ao_from = (INFO_ARG)->ao_from;   \
            AdjectiveOrder ao_to = (INFO_ARG)->ao_to;       \
            for (size_t i = 0; i + 1 < (ARR).size(); ++i) { \
                if(ao_from != ao_to && (((ARR)[i].type == NOUN && (ARR)[i + 1].type == ADJECTIVE) || ((ARR)[i].type == ADJECTIVE && (ARR)[i + 1].type == NOUN))){ \
                  Word temp_word_i = (ARR)[i];\
                  Word temp_word_i_ = (ARR)[i + 1];\
                  if(ao_from == ADJECTIVE_FIRST && ao_to == NOUN_FIRST && (ARR)[i].type == ADJECTIVE && (ARR)[i + 1].type == NOUN){\
                  (ARR)[i] = temp_word_i_;     \
                  (ARR)[i + 1] = temp_word_i;   \
                  }\
                  else if(ao_from == NOUN_FIRST && ao_to == ADJECTIVE_FIRST && (ARR)[i].type == NOUN && (ARR)[i + 1].type == ADJECTIVE) {\
                  (ARR)[i] = temp_word_i_;     \
                  (ARR)[i + 1] = temp_word_i;   \
                  }\
                }\
              }\
        }\
if ((ARR).size() >= 2) { \
for (size_t i = 0; i < (ARR).size(); ++i) { \
    if (i + 1 < (ARR).size() && (ARR)[i].type == PRONOUN && (ARR)[i + 1].type == VERB && ((ARR)[i + 1].flags) & DATIVE_CONST) { \
         (ARR)[i].translation = find_relation((ARR)[i], OBLIQUE); \
    } \
}\
    bool has_agreement_morph = false; \
    for (const auto& var : MORPH_TO_OBJ.variations) { \
        if (var.agreement == AGREEMENT) { \
            has_agreement_morph = true; \
            break; \
        } \
    } \
    if (has_agreement_morph) { \
        Vector<int> word_applied_mask((ARR).size(), 0); \
        for (size_t i = 0; i < (ARR).size(); ++i) { \
            if ((ARR)[i].type == NOUN || (ARR)[i].type == PRONOUN) { \
                if ((ARR)[i].translation.empty() || (ARR)[i].translation == (ARR)[i].word) { \
                    continue; \
                } \
                uint64_t noun_flags = (ARR)[i].flags; \
                Vector<const MorphVariation*> morphs_to_apply; \
                for (const auto& var : MORPH_TO_OBJ.variations) { \
                    if (var.agreement == AGREEMENT && (noun_flags & var.flag)) { \
                        morphs_to_apply.push_back(&var); \
                    } \
                } \
                for (const auto& var : MORPH_TO_OBJ.variations) { \
                    if (var.agreement == AGREEMENT) { \
                        if (var.type == SUFFIX) { \
                            if ((ARR)[i].translation.size() >= var.ending.size() && \
                                (ARR)[i].translation.compare((ARR)[i].translation.size() - var.ending.size(), var.ending.size(), var.ending) == 0) { \
                                bool already_added = false; \
                                for (const auto& added : morphs_to_apply) { \
                                    if (added->morphology == var.morphology && added->flag == var.flag) { \
                                        already_added = true; \
                                        break; \
                                    } \
                                } \
                                if (!already_added) { \
                                    morphs_to_apply.push_back(&var); \
                                } \
                            } \
                        } \
                    } \
                } \
                if (morphs_to_apply.size() >= 2) { \
                    do { \
                        size_t n = morphs_to_apply.size(); \
                        for (size_t ii = 0; ii < n - 1; ii++) { \
                            for (size_t jj = 0; jj < n - ii - 1; jj++) { \
                                const MorphVariation* a = morphs_to_apply[jj]; \
                                const MorphVariation* b = morphs_to_apply[jj + 1]; \
                                bool swap_needed = false; \
                                if (a->form.empty() && !b->form.empty()) { \
                                    swap_needed = true; \
                                } else if (!a->form.empty() && b->form.empty()) { \
                                    swap_needed = false; \
                                } else if (a->morphology > b->morphology) { \
                                    swap_needed = true; \
                                } \
                                if (swap_needed) { \
                                    const MorphVariation* temp = morphs_to_apply[jj]; \
                                    morphs_to_apply[jj] = morphs_to_apply[jj + 1]; \
                                    morphs_to_apply[jj + 1] = temp; \
                                } \
                            } \
                        } \
                    } while(0); \
                } \
                if (!morphs_to_apply.empty()) { \
                    for (size_t j = i + 1; j < (ARR).size(); ++j) { \
                        if ((ARR)[j].type == ADJECTIVE || (ARR)[j].type == ARTICLE || (ARR)[j].type == PRONOUN) { \
                            String translation = (ARR)[j].translation; \
                            for (const auto& morph : morphs_to_apply) { \
                                if (!(word_applied_mask[j] & (1 << morph->morphology))) { \
                                    translation = apply_morph(translation, morph, &MORPH_TO_OBJ, 0); \
                                    word_applied_mask[j] |= (1 << morph->morphology); \
                                } \
                            } \
                            (ARR)[j].translation = translation; \
                        } \
                    } \
                    for (int j = i - 1; j >= 0; --j) { \
                        if ((ARR)[j].type == ADJECTIVE || (ARR)[j].type == ARTICLE || (ARR)[j].type == PRONOUN) { \
                            String translation = (ARR)[j].translation; \
                            for (const auto& morph : morphs_to_apply) { \
                                if (!(word_applied_mask[j] & (1 << morph->morphology))) { \
                                    translation = apply_morph(translation, morph, &MORPH_TO_OBJ, 0); \
                                    word_applied_mask[j] |= (1 << morph->morphology); \
                                } \
                            } \
                            (ARR)[j].translation = translation; \
                        } \
                    } \
                } \
            } \
        } \
    } \
} \
} while(0)
    
enum WordType {
    NOUN = 0,
    ADJECTIVE = 1,
    VERB = 3,
    INTRANSITIVE_VERB = 36,
    INFINITIVE_VERB = 37,
    PAST_TENSE_VERB = 38,
    PRESENT_CONTINUOUS_VERB = 38,
    PRONOUN = 4,
    PARTICLE = 67, 
    OBLIQUE_PRONOUN_ = 11,
    PREPOSITION = 8,
    ARTICLE = 9,
    ADVERB = 13,
    POSSESSIVE_PRONOUN = 40,
    UNKNOWN = 99
};

enum VerbBases {
    DATIVE_CONST = 1 << 0,
    REFLEXIVE = 1 << 1
};


enum PossessionWordOrder {
    OWNER_FIRST = 0,      // B * A  (Japanese: 犬の歯)
    POSSESSED_FIRST = 1,  // A * B  (Portuguese: dente do cachorro)
};


enum ClauseOrders {
    SVO = 0,
    SOV = 1,
    VSO = 2,
    VOS = 3
};



inline WordType typeFromString(const String& s) {
    if (s == "NOUN") return NOUN;
    if (s == "ADJECTIVE") return ADJECTIVE;
    if (s == "VERB") return VERB;
    if (s == "INTRANSITIVE_VERB") return INTRANSITIVE_VERB;
    if (s == "PRONOUN") return PRONOUN;
    if (s == "PREPOSITION") return PREPOSITION;
    if (s == "ARTICLE") return ARTICLE;
    if (s == "ADVERB") return ADVERB;
    if (s == "POSSESSIVE_PRONOUN") return POSSESSIVE_PRONOUN;

    return UNKNOWN;
}



enum Flags : uint64_t {
    // Common grammar flags (bits 0-15)
    w = 1ULL << 0,
    NO_PLURAL = 1ULL << 1,
    IRREGULAR_PLURAL = 1ULL << 2,
    FIRST_PERSON = 1ULL << 3,
    SECOND_PERSON = 1ULL << 4,
    UNCOUNTABLE = 1ULL << 5,
    FEMININE_GENDER = 1ULL << 6,
    MASCULINE_GENDER = 1ULL << 7,
    CONJUNCTIVE = 1ULL << 8,
    CONTRASTIVE = 1ULL << 9,
    DISJUNCTIVE = 1ULL << 10,
    INDEFINITE = 1ULL << 11,
    PLURAL_NUMBER = 1ULL << 12,
    NUMBER = 1ULL << 13,
    OBLIQUE = 1ULL << 14,
    ANIMATE = 1ULL << 15,
    AGENT = 1ULL << 16,
    TOPIC = 1ULL << 17,
    OBJECT = 1ULL << 18,
    NOT_DECIDED_3 = 1ULL << 19,
    NOT_DECIDED_4 = 1ULL << 20,
    NOT_DECIDED_5 = 1ULL << 21,
    NOT_DECIDED_6 = 1ULL << 22,
    NOT_DECIDED_7 = 1ULL << 23,
    NOT_DECIDED_8 = 1ULL << 24,
    NOT_DECIDED_9 = 1ULL << 25,
    NOT_DECIDED_10 = 1ULL << 26,
    NOT_DECIDED_11 = 1ULL << 27,
    NOT_DECIDED_12 = 1ULL << 28,
    NOT_DECIDED_13 = 1ULL << 29,
    NOT_DECIDED_14 = 1ULL << 30,
    NOT_DECIDED_15 = 1ULL << 31,
    // these free bits are arbitrary flags you can use for lnaguges that have specific odd stuff.
    FREE_BIT_0 = 1ULL << 32,
    FREE_BIT_1 = 1ULL << 33,
    FREE_BIT_2 = 1ULL << 34,
    FREE_BIT_3 = 1ULL << 35,
    FREE_BIT_4 = 1ULL << 36,
    FREE_BIT_5 = 1ULL << 37,
    FREE_BIT_6 = 1ULL << 38,
    FREE_BIT_7 = 1ULL << 39,
    FREE_BIT_8 = 1ULL << 40,
    FREE_BIT_9 = 1ULL << 41,
    FREE_BIT_10 = 1ULL << 42,
    FREE_BIT_11 = 1ULL << 43,
    FREE_BIT_12 = 1ULL << 44,
    FREE_BIT_13 = 1ULL << 45,
    FREE_BIT_14 = 1ULL << 46,
    FREE_BIT_15 = 1ULL << 47,
    FREE_BIT_16 = 1ULL << 48,
    FREE_BIT_17 = 1ULL << 49,
    FREE_BIT_18 = 1ULL << 50,
    FREE_BIT_19 = 1ULL << 51,
    FREE_BIT_20 = 1ULL << 52,
    FREE_BIT_21 = 1ULL << 53,
    FREE_BIT_22 = 1ULL << 54,
    FREE_BIT_23 = 1ULL << 55,
    FREE_BIT_24 = 1ULL << 56,
    FREE_BIT_25 = 1ULL << 57,
    FREE_BIT_26 = 1ULL << 58,
    FREE_BIT_27 = 1ULL << 59,
    FREE_BIT_28 = 1ULL << 60,
    FREE_BIT_29 = 1ULL << 61,
    FREE_BIT_30 = 1ULL << 62,
    FREE_BIT_31 = 1ULL << 63
};

enum VerbFlags : uint64_t {
    // TENSE
    INFINITIVE = 1ULL << 0,
    PRESENT = 1ULL << 1,
    PAST = 1ULL << 2,
    FUTURE = 1ULL << 3,
    CONTINUOUS = 1ULL << 4,
    
    // MOOD
    INDICATIVE = 1ULL << 5,
    SUBJUNCTIVE = 1ULL << 6,
    IMPERATIVE = 1ULL << 7,
    CONDITIONAL = 1ULL << 8,
    
    // ASPECT
    SIMPLE = 1ULL << 9,
    PERFECTIVE = 1ULL << 10,
    IMPERFECTIVE = 1ULL << 11,
    PROGRESSIVE = 1ULL << 12,
    
    // VOICE
    ACTIVE = 1ULL << 13,
    PASSIVE = 1ULL << 14,
    
    // ANIMACY
    INANIMATE = 1ULL << 15,
    ANIMATE_V = 1ULL << 16, 
    
    // PERSON
    FIRST_PERSON_V = 1ULL << 17,  
    SECOND_PERSON_V = 1ULL << 18,  
    THIRD_PERSON = 1ULL << 19,
    FOURTH_PERSON = 1ULL << 20,
    ZERO_PERSON = 1ULL << 21,
    
    // NUMBER
    SINGULAR = 1ULL << 22,
    PLURAL_V = 1ULL << 23,         
    DUAL = 1ULL << 24,
    
    // GENDER
    MASCULINE = 1ULL << 25,
    FEMININE_V = 1ULL << 26,    
    NEUTER = 1ULL << 27,
    
    // FREE BITS
    FREE_BIT_1_V = 1ULL << 28,
    FREE_BIT_2_V = 1ULL << 29,
    
    // VERB FLAGS
    MODAL_VERB = 1ULL << 30,
    AUXILIARY_VERB = 1ULL << 31,
    CAUSATIVE_VERB = 1ULL << 32,
    INTRANSITIVE_VERB_ = 1ULL << 33,
    TRANSITIVE_VERB = 1ULL << 34,
    DITRANSITIVE_VERB = 1ULL << 35,
    REFLEXIVE_VERB = 1ULL << 36,
    PASSIVIZABLE_VERB = 1ULL << 37,
    FOCUS_SENSITIVE_VERB = 1ULL << 38,
    STATIVE_VERB = 1ULL << 39,
    IRREGULAR_CONJUGATION = 1ULL << 40,
};

#define NEUTRAL_GENDER (FEMININE_GENDER | MASCULINE_GENDER) // haha
#define THIRD_PERSON (FIRST_PERSON | SECOND_PERSON) // haha2

enum GrammaticalCase : uint8_t {
    NOMINATIVE      = 1 << 0,
    ACCUSATIVE      = 1 << 1,
    GENITIVE        = 1 << 2,
    DATIVE          = 1 << 3,
    INSTRUMENTAL    = 1 << 4,
    PREPOSITIONAL   = 1 << 5, 
    VOCATIVE        = 1 << 6,
    ABLATIVE        = 1 << 7
};

enum SuffixFlags: uint8_t {
    PLURAL = 1 << 0,
    FEMININE = 1 << 1
};

typedef struct {
    
    String root;
    Vector<String> suffixes;
} SuffixRule;

typedef struct {
  String original;
  String replacement;
} Normalization;

typedef struct {
   String word;
   String translation;
   int type;
   uint64_t orig_flags; // flags for original word, just remembered that in languages that have same linguistic features but with variation (e.g gender in portuguese and russian) we need to know the flags for both the original word and the translation to make decisions.
   uint64_t flags;
   String trigger_word;
} Word;

struct Outcome {
    const char* word;
    float score;
    int type;

       constexpr Outcome(const char* w, int t )
        : word(w), score(0.0f), type(t) {}
};

struct Homonym {
    const char* word;
    Outcome* outcomes; 
    size_t num_outcomes;
    const char** tokens;
    const int* forbidden_previous_type;
    size_t num_tokens;
};

template <size_t N>
Vector<Word> MEDIATE_HOMONYMS(
    Vector<Word> arr,
    const Vector<String>& words,
    Homonym (&homonyms)[N]
) {
    for (size_t i = 0; i < arr.size(); ++i) {

        for (const auto& w : words) {

            if (arr[i].word == w) {

                int start = std::max(0, static_cast<int>(i) - 2);
                int end   = std::min(
                    static_cast<int>(arr.size()) - 1,
                    static_cast<int>(i) + 2
                );

                Vector<String> context;
                for (int j = start; j <= end; ++j) {
                    context.push_back(arr[j].translation);
                }

                size_t contextIndex = static_cast<size_t>(i - start);

                Vector<String> this_context = context;
                Vector<int> word_types(context.size(), 0);

                this_context[contextIndex] = arr[i].word;
                word_types[contextIndex]   = arr[i].type;

                /* check if any context word is untranslated (matches its original word) */
                bool has_untranslated = false;
                for (size_t k = 0; k < this_context.size(); ++k) {
                    if (k != contextIndex && this_context[k] == arr[start + k].word) {
                        has_untranslated = true;
                        break;
                    }
                }
                
                if (has_untranslated) {
                    /* skip homonym resolution, keep default translation */
                    continue;
                }

                String resolved_word =
                    semantics(this_context,
                              word_types,
                              contextIndex,
                              homonyms,   
                              N);        

                if (!resolved_word.empty()) {
                    arr[i].translation = resolved_word;
                }
            }
        }
    }

    return arr;
}



template <size_t N>
inline uint64_t lookupFlags(const Entry (&dict)[N], const char* word) {
    for (size_t i = 0; i < N; ++i) {
        if (strcmp(dict[i].w, word) == 0) {
            return dict[i].flags;
        }
    }
    return 0;
}

inline uint64_t lookupFlags_test(const Entry* dict, size_t count, const char* word) {
    for (size_t i = 0; i < count; ++i) {
        const char* p = dict[i].w;
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) {
            return dict[i].flags;
        }
        
        if (dict[i].w2 && dict[i].w2[0]) {
            p = dict[i].w2;
            q = word;
            while (*p && *q && *p == *q) { ++p; ++q; }
            if (*p == *q) {
                return dict[i].flags;
            }
        }
        
        if (dict[i].w3 && dict[i].w3[0]) {
            p = dict[i].w3;
            q = word;
            while (*p && *q && *p == *q) { ++p; ++q; }
            if (*p == *q) {
                return dict[i].flags;
            }
        }
    }
    return 0;
}

enum FlagTarget {
    CHECK_FIRST_WORD,
    CHECK_SECOND_WORD,
    CHECK_BOTH_WORDS,
    CHECK_EITHER_WORD
};

struct ParticleRule {
    int type_before;
    int type_after;
    const char* particle;
    bool sentence_size;
     uint64_t required_flag;
    FlagTarget flag_target;
    const char* particle_translation;  
    int particle_type;                
};

inline Vector<Word> INSERT_PARTICLE(
    Vector<Word> arr,
    int type_before,
    int type_after,
    const char* particle_word,
    bool sentence_size,
    uint64_t required_flag,
    FlagTarget flag_target, 
    const char* particle_translation = nullptr,
    int particle_type = -1
) {
    Vector<Word> result;
    
    for (size_t i = 0; i < arr.size(); ++i) {
        if (i + 1 < arr.size() && 
            arr[i].type == type_before && 
            arr[i + 1].type == type_after && sentence_size) {
            
            // Check based on target
            bool flag_matches = false;
            switch (flag_target) {
                case CHECK_FIRST_WORD:
                    flag_matches = ((arr[i].flags & required_flag) == required_flag);
                    break;
                case CHECK_SECOND_WORD:
                    flag_matches = ((arr[i + 1].flags & required_flag) == required_flag);
                    break;
                case CHECK_BOTH_WORDS:
                    flag_matches = ((arr[i].flags & required_flag) == required_flag) &&
                                   ((arr[i + 1].flags & required_flag) == required_flag);
                    break;
                case CHECK_EITHER_WORD:
                    flag_matches = ((arr[i].flags & required_flag) == required_flag) ||
                                   ((arr[i + 1].flags & required_flag) == required_flag);
                    break;
            }
            if (flag_matches) {
                result.push_back(arr[i]); 
                result.push_back(Word{
                    particle_word, 
                    particle_translation ? particle_translation : particle_word, 
                    particle_type
                }); 
                result.push_back(arr[i + 1]); 
                i++; 
            } else {
                result.push_back(arr[i]);
            }
        } else {
            result.push_back(arr[i]);
        }
    }
    
    return result;
}



inline Vector<Word> INSERT_PARTICLES(
    Vector<Word> arr, 
    const Vector<ParticleRule>& rules
) {
    
    for (const auto& rule : rules) {
               
        arr = INSERT_PARTICLE(
            arr, 
            rule.type_before, 
            rule.type_after, 
            rule.particle, 
            rule.sentence_size,
            rule.required_flag,
            rule.flag_target,
            rule.particle_translation, 
            rule.particle_type       
        );
    }
    
    return arr;
}


inline Vector<Word> POST_CONJUGATION(
    Vector<Word> arr
   
) {
    Vector<Word> result;
    
    for (size_t i = 0; i < arr.size(); ++i) {
        if (i + 1 < arr.size()) {
            
        } else {
            result.push_back(arr[i]);
        }
    }
    
    return result;
}



// making a decent API for the pairs to access and make implementing easier, but i know i'll keep changing it and never be satisfied
// also, the reason as to why the flag lookups functions are all separate is because i implemented bit masking after having most of the shit done
// i could return the whole struct and access jjust waht i need but i'm sooooooooooooooooooooo lazy.
template <size_t N>
inline const char* lookup(const Entry (&dict)[N], const char* word, int script_index = 0) {
    for (size_t i = 0; i < N; ++i) {
        const char* p = dict[i].w;
        if (!p) continue;  
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) {
            if (script_index == 0) return dict[i].t;
            if (script_index == 1 && dict[i].t2 && dict[i].t2[0]) return dict[i].t2;
            if (script_index == 2 && dict[i].t3 && dict[i].t3[0]) return dict[i].t3;
            return dict[i].t; 
        }
        
        if (dict[i].w2 && dict[i].w2[0]) {
            p = dict[i].w2;
            q = word;
            while (*p && *q && *p == *q) { ++p; ++q; }
            if (*p == *q) {
                if (script_index == 0) return dict[i].t;
                if (script_index == 1 && dict[i].t2 && dict[i].t2[0]) return dict[i].t2;
                if (script_index == 2 && dict[i].t3 && dict[i].t3[0]) return dict[i].t3;
                return dict[i].t;
            }
        }
        
        if (dict[i].w3 && dict[i].w3[0]) {
            p = dict[i].w3;
            q = word;
            while (*p && *q && *p == *q) { ++p; ++q; }
            if (*p == *q) {
                if (script_index == 0) return dict[i].t;
                if (script_index == 1 && dict[i].t2 && dict[i].t2[0]) return dict[i].t2;
                if (script_index == 2 && dict[i].t3 && dict[i].t3[0]) return dict[i].t3;
                return dict[i].t;
            }
        }
    }
    return nullptr;
}

inline const char* lookup_test(const Entry* dict, size_t count, const char* word, int script_index = 0)
{
    for (size_t i = 0; i < count; ++i) {
        const char* p = dict[i].w;
        if (!p) continue;  
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) {
            if (script_index == 0) return dict[i].t;
            if (script_index == 1 && dict[i].t2 && dict[i].t2[0]) return dict[i].t2;
            if (script_index == 2 && dict[i].t3 && dict[i].t3[0]) return dict[i].t3;
            return dict[i].t; 
        }
        
        if (dict[i].w2 && dict[i].w2[0]) {
            p = dict[i].w2;
            q = word;
            while (*p && *q && *p == *q) { ++p; ++q; }
            if (*p == *q) {
                if (script_index == 0) return dict[i].t;
                if (script_index == 1 && dict[i].t2 && dict[i].t2[0]) return dict[i].t2;
                if (script_index == 2 && dict[i].t3 && dict[i].t3[0]) return dict[i].t3;
                return dict[i].t;
            }
        }
        
        if (dict[i].w3 && dict[i].w3[0]) {
            p = dict[i].w3;
            q = word;
            while (*p && *q && *p == *q) { ++p; ++q; }
            if (*p == *q) {
                if (script_index == 0) return dict[i].t;
                if (script_index == 1 && dict[i].t2 && dict[i].t2[0]) return dict[i].t2;
                if (script_index == 2 && dict[i].t3 && dict[i].t3[0]) return dict[i].t3;
                return dict[i].t;
            }
        }
    }
    return nullptr;
}


template <size_t N>
inline const char* auto_correct(const Entry(&dict)[N] , String word) {
   String result = word;
   String word_temp = word;
   
   //try different letters at every position (femilia -> familia)
   String possible_letters = "abcdefghjijklmnopqrstuvwxyz";
   for(size_t i = 0; i < word.length(); i++){
    for(size_t j = 0; j < possible_letters.length(); j++){
      word_temp.replace(i, 1, 1, possible_letters[j]);
      const char* lookup_ = lookup(dict, word_temp.c_str());
      if(lookup_ && String(lookup_).length() > 2) {
        return lookup_;
    }else{
        //reset after failed 
        word_temp = word;
    };
    }
   }

   //now we try to add a letter at each given position (fmilia -> familia)
  for(size_t i = 0; i < word.length(); i++){
    for(size_t j = 0; j < possible_letters.length(); j++){
      word_temp.insert(word_temp.begin() + i, possible_letters[j]);
      const char* lookup_ = lookup(dict, word_temp.c_str());
      if(lookup_ && String(lookup_).length() > 2) {
        return lookup_;
    }else{
        //reset after failed 
        word_temp = word;
    };
    }
   }

   //now we try to remove extra letters at any given position (faamilia -> familia)
        for(size_t i = 0; i < word.length(); i++){
             word_temp.erase(i, 1);
                const char* lookup_ = lookup(dict, word_temp.c_str());
            if(lookup_ && String(lookup_).length() > 2) {
                        return lookup_;
            }else{
                        //reset after failed 
                        word_temp = word;
            };
        }

    return nullptr;
}


template <size_t N>
inline Verb verb_lookup(const Verb (&dict)[N], const char* word) {
    for (size_t i = 0; i < N; ++i) {
        const char* p = dict[i].w;
        if (!p) continue;  
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) return Verb{
            dict[i].w,
            dict[i].translation,
            dict[i].type,      
            dict[i].flags
        };
    }
    return Verb{
            "",       
            "", 
            0,      
            0
        };
}
inline Verb find_verb_in_array(const Verb dict[], size_t count, const char* word) {
    
    for (size_t i = 0; i < count; ++i) {
        const char* p = dict[i].w;
        if (!p) continue;
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == '\0' && *q == '\0')
            return dict[i];
    }
    return Verb{"", "", 0, 0};
}


template <size_t N>
inline uint8_t lookupVerbFlags(const Verb (&dict)[N], const char* word) {
    for (size_t i = 0; i < N; ++i) {
        const char* p = dict[i].w;
        if (!p) continue;  
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) return dict[i].flags;
    }
    return 0;
}


inline uint64_t lookupVerbFlags_test(const Verb* dict, size_t count, const char* word) {
    for (size_t i = 0; i < count; ++i) {
        const char* p = dict[i].w;
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) {
            return dict[i].flags;
        }
        
        if (dict[i].w2 && dict[i].w2[0]) {
            p = dict[i].w2;
            q = word;
            while (*p && *q && *p == *q) { ++p; ++q; }
            if (*p == *q) {
                return dict[i].flags;
            }
        }
        
        if (dict[i].w3 && dict[i].w3[0]) {
            p = dict[i].w3;
            q = word;
            while (*p && *q && *p == *q) { ++p; ++q; }
            if (*p == *q) {
                return dict[i].flags;
            }
        }
    }
    return 0;
}


template <size_t N>
Suffix lookupSuff(const Suffix (&dict)[N], const char* word) {
    for (size_t i = 0; i < N; ++i) {
        const char* p = dict[i].w;
        if (!p) continue;  
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }

        if (*p == *q) {
            return dict[i];
        }
    }
    return Suffix{nullptr, nullptr, 99, 0};
}

template <size_t N>
inline uint8_t lookupSuffFlags(const Suffix (&dict)[N], const char* word) {
    for (size_t i = 0; i < N; ++i) {
        const char* p = dict[i].w;
        if (!p) continue;  
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) return dict[i].flags;
    }
    return 0;
}





//invert a pair (casa azul -> azul casa)
inline void invert(Vector<Word>& output, const Word& first, const Word& second, const char*) {
    
    if (!output.empty()) output.pop_back(); 
    output.push_back(first);
    output.push_back(second);
}
//
inline void replace_first(Vector<Word>& output, const Word& replacement, const Word& second, const char*) {
    if (!output.empty()) output.pop_back();

    output.push_back(replacement);
    output.push_back(second);
}

// inject a word in the middle of two words (orange juice -> suco de laranja)
inline void sandwich(Vector<Word>& output, const Word& first, const Word& second, const char* word) {
    if (!output.empty()) output.pop_back(); 
    output.push_back(first);
    output.push_back(Word{word, word, 0});
    output.push_back(second);
}

// replace a word in the middle of two words (orange juice -> suco de laranja)
inline void sandwich_replace(Vector<Word>& output,const Word& middle, const Word& second, const char*) {
    if (!output.empty()) output.pop_back(); 
    output.push_back(middle);
    output.push_back(second);
}

//remove middle word
inline void remove_middle(Vector<Word>& output, const Word& first, const Word& middle, const Word& last) {
    if (!output.empty()) output.pop_back();
    output.push_back(first);
    output.push_back(last);
}
inline void remove_first(Vector<Word>& output, const Word& first, const Word& second, const char*) {
  if (!output.empty()) {output.pop_back(); }
    output.push_back(first);
}

inline void remove_current(Vector<Word>& output) {
  if (output.size() > 1) {
        output.erase(output.end() - 1, output.end()); 
    }           
}

inline void remove_pair(Vector<Word>& output) {
    if (output.size() >= 2) {
        output.erase(output.end() - 2, output.end()); 
    }
}

struct TransferRule {
    Vector<String> source_pattern;  // e.g., {"é", "*1", "de", "*3"}
    Vector<String> target_pattern;  // e.g., {"*1", "чтобы", "сделать"}
};

Vector<TransferRule> transfers = {
  {
    {"*3", "muito"}, 
    {"*3", "xeka"}
}


};

inline bool isPunctuation(const String &token) {
    if (token.empty()) return false;

    const char punct[] = ".,?!-/:;()[]{}\"'";

    char first = token.front();
    char last  = token.back();

    for (size_t i = 0; i < sizeof(punct) - 1; ++i) {
        if (first == punct[i] || last == punct[i]) {
            return true;
        }
    }

    return false;
}

static Vector<Word> applyTransferRules(const Vector<Word>& copy) {
    Vector<Word> sentence_arr = copy;
    Vector<Word> reordered_arr;
    Vector<bool> consumed(sentence_arr.size(), false);
    
    size_t i = 0;
    while (i < sentence_arr.size()) {
        if (consumed[i]) {
            ++i;
            continue;
        }
        
        bool matched = false;
        
        for (const auto& rule : transfers) {
            size_t pattern_len = rule.source_pattern.size();
            if (i + pattern_len > sentence_arr.size()) continue;
            
            Vector<int> captured_indices;   // stores original positions of captured words
            bool match_failed = false;
            size_t captured_count = 0;      // track how many words were captured by $ (for consumption)
            
            for (size_t j = 0; j < pattern_len; ++j) {
                const String& pattern_token = rule.source_pattern[j];
                const Word& current_word = sentence_arr[i + j];
                
                if (pattern_token == "$") {
                    // Capture all remaining words until punctuation (or end of sentence idk)
                    size_t pos = i + j;
                    while (pos < sentence_arr.size() && !consumed[pos] && !isPunctuation(sentence_arr[pos].word)) {
                        captured_indices.push_back(pos);
                        pos++;
                    }
                    if (captured_indices.empty()) {
                        match_failed = true;
                        break;
                    }
                    captured_count = captured_indices.size();
                    // $ is always the last token; break out
                    break;
                }
                // handle typed wildcard *type&flags 
                else if (pattern_token[0] == '*') {
                    size_t amp_pos = pattern_token.find('&');
                    int required_type;
                    Vector<uint64_t> required_flags;
                    
                    if (amp_pos != String::npos) {
                        String type_str = pattern_token.substr(1, amp_pos - 1);
                        if (type_str.empty()) {
                            match_failed = true;
                            break;
                        }
                        required_type = string_to_int(type_str);
                        
                        size_t start = amp_pos + 1;
                        while (start < pattern_token.size()) {
                            size_t next_amp = pattern_token.find('&', start);
                            String flag_str;
                            if (next_amp != String::npos) {
                                flag_str = pattern_token.substr(start, next_amp - start);
                                start = next_amp + 1;
                            } else {
                                flag_str = pattern_token.substr(start);
                                start = pattern_token.size();
                            }
                            if (!flag_str.empty()) {
                                int flag_bit = string_to_int(flag_str);
                                required_flags.push_back(1ULL << flag_bit);
                            }
                        }
                    } else {
                        String type_str = pattern_token.substr(1);
                        if (type_str.empty()) {
                            match_failed = true;
                            break;
                        }
                        required_type = string_to_int(type_str);
                    }
                    
                    // Check type
                    if (current_word.type != required_type) {
                        match_failed = true;
                        break;
                    }
                    // Check flags
                    for (uint64_t required_flag : required_flags) {
                        if ((current_word.flags & required_flag) == 0) {
                            match_failed = true;
                            break;
                        }
                    }
                    if (match_failed) break;
                    
                    // capture this word
                    captured_indices.push_back(i + j);
                }
                // hndle literal match
                else {
                    if (current_word.word != pattern_token) {
                        match_failed = true;
                        break;
                    }
                    // Literals are not captured; they are just checked
                }
            }
            
            // If pattern matched and we captured at least one wildcard
            if (!match_failed && !captured_indices.empty()) {
                // process target pattern 
                for (const String& target_token : rule.target_pattern) {
                    if (target_token == "$") {
                        // Insert all captured words (in capture order)
                        for (int idx : captured_indices) {
                            reordered_arr.push_back(sentence_arr[idx]);
                        }
                    }
                    else if (target_token[0] == '*') {
                        // Target wildcard: extract the type number and find the captured word with that type
                        String type_str = target_token.substr(1);
                        if (type_str.empty()) continue;
                        int required_type = string_to_int(type_str);
                        
                        // Search for a captured word with this type
                        bool found = false;
                        for (size_t k = 0; k < captured_indices.size(); ++k) {
                            int pos = captured_indices[k];
                            if (sentence_arr[pos].type == required_type) {
                                reordered_arr.push_back(sentence_arr[pos]);
                                found = true;
                                break;
                            }
                        }
                        // If not found, skip (no output)
                    }
                    else {
                        // Literal token in target
                        Word new_word;
                        new_word.word = target_token;
                        new_word.translation = target_token;
                        new_word.type = -1;
                        new_word.flags = 0;
                        new_word.trigger_word = "";
                        reordered_arr.push_back(new_word);
                    }
                }
                
                // If pattern had $, it consumed more than pattern_len words
                size_t total_consumed = pattern_len;
                if (captured_count > 0) {
                    total_consumed = captured_indices.size();  // $ captured variable number
                }
                
                // Mark all words that matched the pattern as consumed
                for (size_t j = 0; j < total_consumed; ++j) {
                    consumed[i + j] = true;
                }
                
                matched = true;
                i += total_consumed;  // skip all consumed words
                break;
            }
        }
        
        if (!matched) {
            // No rule matched: keep the current word as-is
            reordered_arr.push_back(sentence_arr[i]);
            ++i;
        }
    }
    
    return reordered_arr;
}


// New rule structure for morphophonological rules
struct MorphRule {
    Vector<String> source_pattern;     // Pattern to match (like transfer rules)
    Vector<String> target_pattern;     // Target with modifications
    bool case_sensitive;              // For phonological checks
};

// Global rules
Vector<MorphRule> morph_rules = {
    {
        {"*&34", "*&6*7"},    // Pattern: FREE_BIT_2, then NEUTER
        {"*&6*7", "*&34"}     // Target: NEUTER, then FREE_BIT_2 (SWAP!)
    }
};



struct PatternCondition {
    enum Type {
        COND_NONE,
        COND_STARTS_VOWEL,
        COND_STARTS_CONSONANT,
        COND_ENDS_VOWEL,
        COND_ENDS_CONSONANT,
        COND_CONTAINS_VOWEL,
        COND_CONTAINS_CONSONANT,
        COND_STARTS_WITH,
        COND_ENDS_WITH,
        COND_CONTAINS
    };
    Type type;
    char specific_char;
};
void parseTargetToken(const String& token, Vector<int>& types, uint64_t& flags, bool& any_type) {
    types.clear();
    flags = 0;
    any_type = false;
    
    if (token == "*") {
        any_type = true;
        return;
    }
    
    String remaining = token.substr(1);
    
    // Remove modifications
    size_t mod_pos = remaining.find_first_of("+-/~");
    if (mod_pos != String::npos) {
        remaining = remaining.substr(0, mod_pos);
    }
    
    // Parse flags first
    size_t flag_pos = remaining.find('&');
    if (flag_pos != String::npos) {
        size_t pos = flag_pos;
        while (pos != String::npos && pos + 1 < remaining.size()) {
            size_t next = remaining.find('&', pos + 1);
            String flag_str = (next != String::npos) ? remaining.substr(pos + 1, next - pos - 1) : remaining.substr(pos + 1);
            if (!flag_str.empty()) {
                int flag_bit = string_to_int(flag_str);
                flags |= (1ULL << flag_bit);
            }
            pos = next;
        }
        remaining = remaining.substr(0, flag_pos);
    }
    
    // Parse types
    if (remaining.empty()) {
        any_type = true;
    } else {
        size_t start = 0;
        while (start < remaining.size()) {
            size_t end = remaining.find('*', start);
            String type_str = (end != String::npos) ? remaining.substr(start, end - start) : remaining.substr(start);
            if (!type_str.empty()) {
                types.push_back(string_to_int(type_str));
            }
            start = (end != String::npos) ? end + 1 : remaining.size();
        }
        any_type = false;
    }
}

// Parse condition from string like "^V", "$C", "{a}"
PatternCondition parseCondition(const String& cond_str) {
    PatternCondition cond;
    cond.type = PatternCondition::COND_NONE;
    cond.specific_char = '\0';
    
    if (cond_str.empty()) return cond;
    
    // Check for ^ (starts with)
    if (cond_str[0] == '^' && cond_str.size() > 1) {
        char marker = cond_str[1];
        if (marker == 'V') {
            cond.type = PatternCondition::COND_STARTS_VOWEL;
        } else if (marker == 'C') {
            cond.type = PatternCondition::COND_STARTS_CONSONANT;
        } else if (isalpha(marker)) {
            cond.type = PatternCondition::COND_STARTS_WITH;
            cond.specific_char = tolower(marker);
        }
        return cond;
    }
    
    // Check for $ (ends with)
    if (cond_str[0] == '$' && cond_str.size() > 1) {
        char marker = cond_str[1];
        if (marker == 'V') {
            cond.type = PatternCondition::COND_ENDS_VOWEL;
        } else if (marker == 'C') {
            cond.type = PatternCondition::COND_ENDS_CONSONANT;
        } else if (isalpha(marker)) {
            cond.type = PatternCondition::COND_ENDS_WITH;
            cond.specific_char = tolower(marker);
        }
        return cond;
    }
    
    // Check for { } (contains)
    if (cond_str[0] == '{' && cond_str.size() > 2 && cond_str[2] == '}') {
        char marker = cond_str[1];
        if (marker == 'V') {
            cond.type = PatternCondition::COND_CONTAINS_VOWEL;
        } else if (marker == 'C') {
            cond.type = PatternCondition::COND_CONTAINS_CONSONANT;
        } else if (isalpha(marker)) {
            cond.type = PatternCondition::COND_CONTAINS;
            cond.specific_char = tolower(marker);
        }
        return cond;
    }
    
    return cond;
}

struct PatternToken {
    Vector<int> types;          // Allowed types (OR logic)
    uint64_t required_flags;    // Flags that must be set (AND logic)
    PatternCondition cond;      // Phonological condition
    bool any_type;
};

void parsePatternToken(const String& token, PatternToken& result) {
    result.types.clear();
    result.required_flags = 0;
    result.cond.type = PatternCondition::COND_NONE;
    result.cond.specific_char = '\0';
    result.any_type = false;
    
    if (token == "*") {
        result.any_type = true;
        return;
    }
    
    // Remove leading '*'
    String remaining = token.substr(1);
    
    // Find where conditions start (^, $, {) or flags (&)
    size_t cond_pos = remaining.find_first_of("^${");
    size_t flag_pos = remaining.find('&');
    size_t type_end = remaining.size();
    
    if (cond_pos != String::npos && cond_pos < type_end) type_end = cond_pos;
    if (flag_pos != String::npos && flag_pos < type_end) type_end = flag_pos;
    
    // Parse types (e.g., "3*9" -> types 3 and 9)
    String type_part = remaining.substr(0, type_end);
    if (type_part.empty()) {
        result.any_type = true;
    } else {
        size_t start = 0;
        while (start < type_part.size()) {
            size_t end = type_part.find('*', start);
            String type_str;
            if (end != String::npos) {
                type_str = type_part.substr(start, end - start);
                start = end + 1;
            } else {
                type_str = type_part.substr(start);
                start = type_part.size();
            }
            if (!type_str.empty()) {
                result.types.push_back(string_to_int(type_str));
            }
        }
        result.any_type = false;
    }
    
    // Parse flags (&1&2&3)
    size_t pos = flag_pos;
    while (pos != String::npos && pos + 1 < remaining.size()) {
        size_t next = remaining.find('&', pos + 1);
        String flag_str;
        if (next != String::npos) {
            flag_str = remaining.substr(pos + 1, next - pos - 1);
            pos = next;
        } else {
            flag_str = remaining.substr(pos + 1);
            pos = String::npos;
        }
        if (!flag_str.empty()) {
            int flag_bit = string_to_int(flag_str);
            result.required_flags |= (1ULL << flag_bit);
        }
    }
    
    // Parse condition (^, $, {})
    if (cond_pos != String::npos) {
        String cond_str = remaining.substr(cond_pos);
        result.cond = parseCondition(cond_str);
    }
}

// ============================================================
// PHONOLOGICAL HELPER FUNCTIONS
// ============================================================

inline bool endsWithVowel(const String& word) {
    if (word.empty()) return false;
    char last = tolower(word[word.length() - 1]);
    return last == 'a' || last == 'e' || last == 'i' || last == 'o' || last == 'u' ||
           last == 'á' || last == 'é' || last == 'í' || last == 'ó' || last == 'ú' ||
           last == 'ã' || last == 'õ' || last == 'â' || last == 'ê' || last == 'ô';
}

inline bool endsWithConsonant(const String& word) {
    if (word.empty()) return false;
    return !endsWithVowel(word) && isalpha(word[word.length() - 1]);
}

inline bool startsWithVowel(const String& word) {
    if (word.empty()) return false;
    char first = tolower(word[0]);
    return first == 'a' || first == 'e' || first == 'i' || first == 'o' || first == 'u' ||
           first == 'á' || first == 'é' || first == 'í' || first == 'ó' || first == 'ú' ||
           first == 'ã' || first == 'õ' || first == 'â' || first == 'ê' || first == 'ô';
}

inline bool startsWithConsonant(const String& word) {
    if (word.empty()) return false;
    return !startsWithVowel(word) && isalpha(word[0]);
}

inline bool containsVowel(const String& word) {
    for (char c : word) {
        char lower = tolower(c);
        if (lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u' ||
            lower == 'á' || lower == 'é' || lower == 'í' || lower == 'ó' || lower == 'ú' ||
            lower == 'ã' || lower == 'õ' || lower == 'â' || lower == 'ê' || lower == 'ô') {
            return true;
        }
    }
    return false;
}

inline bool containsConsonant(const String& word) {
    for (char c : word) {
        if (isalpha(c)) {
            char lower = tolower(c);
            if (!(lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u' ||
                  lower == 'á' || lower == 'é' || lower == 'í' || lower == 'ó' || lower == 'ú' ||
                  lower == 'ã' || lower == 'õ' || lower == 'â' || lower == 'ê' || lower == 'ô')) {
                return true;
            }
        }
    }
    return false;
}

inline bool matchesCondition(const Word& word, const PatternCondition& cond) {
    const String& text = word.translation;
    if (text.empty()) return false;
    
    switch (cond.type) {
        case PatternCondition::COND_STARTS_VOWEL:
            return startsWithVowel(text);
        case PatternCondition::COND_STARTS_CONSONANT:
            return startsWithConsonant(text);
        case PatternCondition::COND_ENDS_VOWEL:
            return endsWithVowel(text);
        case PatternCondition::COND_ENDS_CONSONANT:
            return endsWithConsonant(text);
        case PatternCondition::COND_CONTAINS_VOWEL:
            return containsVowel(text);
        case PatternCondition::COND_CONTAINS_CONSONANT:
            return containsConsonant(text);
        case PatternCondition::COND_STARTS_WITH:
            return !text.empty() && tolower(text[0]) == cond.specific_char;
        case PatternCondition::COND_ENDS_WITH:
            return !text.empty() && tolower(text[text.length()-1]) == cond.specific_char;
        case PatternCondition::COND_CONTAINS:
            for (char c : text) {
                if (tolower(c) == cond.specific_char) return true;
            }
            return false;
        default:
            return true;
    }
}

// ============================================================
// TARGET MODIFICATION PARSING
// ============================================================

struct TargetModification {
    enum Type {
        MOD_NONE,
        MOD_APPEND,
        MOD_PREPEND,
        MOD_REPLACE,
        MOD_DELETE
    };
    Type type;
    String text;
    String search;
};
// Helper functions
// Parse pattern token with types, flags, and conditions
void parsePatternTokenFull(const String& token, Vector<int>& types, uint64_t& flags, PatternCondition& cond, bool& any_type) {
    types.clear();
    flags = 0;
    cond.type = PatternCondition::COND_NONE;
    cond.specific_char = '\0';
    any_type = false;
    
    if (token == "*") {
        any_type = true;
        return;
    }
    
    String remaining = token.substr(1);
    
    size_t cond_pos = remaining.find_first_of("^${");
    size_t flag_pos = remaining.find('&');
    size_t type_end = remaining.size();
    
    if (cond_pos != String::npos && cond_pos < type_end) type_end = cond_pos;
    if (flag_pos != String::npos && flag_pos < type_end) type_end = flag_pos;
    
    // Parse types
    String type_part = remaining.substr(0, type_end);
    if (type_part.empty()) {
        any_type = true;
    } else {
        size_t start = 0;
        while (start < type_part.size()) {
            size_t end = type_part.find('*', start);
            String type_str = (end != String::npos) ? type_part.substr(start, end - start) : type_part.substr(start);
            if (!type_str.empty()) {
                types.push_back(string_to_int(type_str));
            }
            start = (end != String::npos) ? end + 1 : type_part.size();
        }
        any_type = false;
    }
    
    // Parse flags
    size_t pos = flag_pos;
    while (pos != String::npos && pos + 1 < remaining.size()) {
        size_t next = remaining.find('&', pos + 1);
        String flag_str = (next != String::npos) ? remaining.substr(pos + 1, next - pos - 1) : remaining.substr(pos + 1);
        if (!flag_str.empty()) {
            int flag_bit = string_to_int(flag_str);
            flags |= (1ULL << flag_bit);
        }
        pos = next;
    }
    
    // Parse condition
    if (cond_pos != String::npos) {
        String cond_str = remaining.substr(cond_pos);
        cond = parseCondition(cond_str);
    }
}

TargetModification parseModification(const String& token) {
    TargetModification mod;
    mod.type = TargetModification::MOD_NONE;
    
    // Remove leading '*' and type part
    size_t mod_pos = token.find_first_of("+-/~");
    if (mod_pos == String::npos) return mod;
    
    String mod_str = token.substr(mod_pos);
    
    // Check for + (append)
    if (mod_str[0] == '+' && mod_str.size() > 1) {
        mod.type = TargetModification::MOD_APPEND;
        mod.text = mod_str.substr(1);
        return mod;
    }
    
    // Check for - (delete suffix)
    if (mod_str[0] == '-' && mod_str.size() > 1) {
        mod.type = TargetModification::MOD_DELETE;
        mod.text = mod_str.substr(1);
        return mod;
    }
    
    // Check for / (replace)
    if (mod_str[0] == '/') {
        size_t pos1 = mod_str.find('/', 1);
        if (pos1 != String::npos && pos1 + 1 < mod_str.size()) {
            size_t pos2 = mod_str.find('/', pos1 + 1);
            if (pos2 != String::npos && pos2 + 1 < mod_str.size()) {
                mod.type = TargetModification::MOD_REPLACE;
                mod.search = mod_str.substr(1, pos1 - 1);
                mod.text = mod_str.substr(pos1 + 1, pos2 - pos1 - 1);
                return mod;
            }
        }
        return mod;
    }
    
    // Check for ~ (delete entire word)
    if (mod_str[0] == '~') {
        mod.type = TargetModification::MOD_DELETE;
        mod.text = "";
        return mod;
    }
    
    return mod;
}

static String stripConditions(const String& token) {
    if (token.empty() || token[0] != '*') return token;
    
    // Remove conditions (^, $, or {)
    size_t cond_pos = token.find_first_of("^${");
    if (cond_pos != String::npos) {
        return token.substr(0, cond_pos);
    }
    return token;
}

static Vector<Word> applyMorphRules(const Vector<Word>& copy) {
    Vector<Word> sentence_arr = copy;
    Vector<Word> reordered_arr;
    
    Vector<bool> consumed(sentence_arr.size(), false);
    
    size_t i = 0;
    while (i < sentence_arr.size()) {
        if (consumed[i]) {
            ++i;
            continue;
        }
        
        bool matched = false;
        
        for (size_t rule_idx = 0; rule_idx < morph_rules.size(); ++rule_idx) {
            const auto& rule = morph_rules[rule_idx];
            size_t pattern_len = rule.source_pattern.size();
            if (i + pattern_len > sentence_arr.size()) continue;
            
         
            
            Vector<int> captured_indices;
            Vector<String> captured_tokens;
            bool match_failed = false;
            size_t captured_count = 0;
            
            for (size_t j = 0; j < pattern_len; ++j) {
                const String& pattern_token = rule.source_pattern[j];
                const Word& current_word = sentence_arr[i + j];
              
                if (pattern_token == "$") {
                  
                    size_t pos = i + j;
                    while (pos < sentence_arr.size() && !consumed[pos] && !isPunctuation(sentence_arr[pos].word)) {
                        captured_indices.push_back(pos);
                        captured_tokens.push_back(pattern_token);
                        pos++;
                    }
                    if (captured_indices.empty()) {
                       
                        match_failed = true;
                        break;
                    }
                    captured_count = captured_indices.size();
                    break;
                }
                else if (pattern_token[0] == '*') {
                    Vector<int> allowed_types;
                    uint64_t required_flags = 0;
                    PatternCondition cond;
                    bool any_type = false;
                    parsePatternTokenFull(pattern_token, allowed_types, required_flags, cond, any_type);
                    
                
                    
                    // Check type
                    bool type_matches = any_type;
                    if (!type_matches) {
                        for (int allowed_type : allowed_types) {
                            if (current_word.type == allowed_type) {
                                type_matches = true;
                                break;
                            }
                        }
                    }
                    if (!type_matches) {
                        match_failed = true;
                        break;
                    }
                    
                    // Check flags
                    bool flags_match = ((current_word.flags & required_flags) == required_flags);
                   if (!flags_match) {
                        match_failed = true;
                     
                        break;
                    }
                    
                    // Check condition
                    if (cond.type != PatternCondition::COND_NONE) {
                        bool cond_match = matchesCondition(current_word, cond);
                       
                        if (!cond_match) {
                            match_failed = true;
                           
                            break;
                        }
                    }
                    
                    captured_indices.push_back(i + j);
                    captured_tokens.push_back(pattern_token);
                
                }
                else {
                    if (current_word.word != pattern_token) {
                     
                        match_failed = true;
                        break;
                    }
                   
                }
            }
            
            if (match_failed || captured_indices.empty()) {
             
                continue;
            }
            
             for (size_t k = 0; k < captured_indices.size(); ++k) {
                int pos = captured_indices[k];
            
            }
            
            // Process target
          
            for (const String& target_token : rule.target_pattern) {
               
                if (target_token == "$") {
                    for (int idx : captured_indices) {
                        reordered_arr.push_back(sentence_arr[idx]);
                    }
                }
                else if (target_token[0] == '*') {
                    Vector<int> target_types;
                    uint64_t target_flags = 0;
                    bool any_type = false;
                    parseTargetToken(target_token, target_types, target_flags, any_type);
                    
                
                    
                    bool found = false;
                    for (size_t k = 0; k < captured_indices.size(); ++k) {
                        int pos = captured_indices[k];
                        const Word& word = sentence_arr[pos];
                        
                      
                        // Check flags FIRST
                        if ((word.flags & target_flags) != target_flags) {
        
                            continue;
                        }
                        
                        // Check type
                        bool type_matches = any_type;
                        if (!type_matches) {
                            for (int type : target_types) {
                                if (word.type == type) {
                                    type_matches = true;
                                    break;
                                }
                            }
                        }
                        if (!type_matches) {
                            continue;
                        }
                        
                        // Found it!
                       Word new_word = word;
                        TargetModification mod = parseModification(target_token);
                        switch (mod.type) {
                            case TargetModification::MOD_APPEND:
                                new_word.translation += mod.text;
                                new_word.word = new_word.translation;
                                break;
                            case TargetModification::MOD_PREPEND:
                                new_word.translation = mod.text + new_word.translation;
                                new_word.word = new_word.translation;
                                break;
                            case TargetModification::MOD_REPLACE: {
                                size_t start = new_word.translation.find(mod.search);
                                if (start != String::npos) {
                                    new_word.translation.replace(start, mod.search.size(), mod.text);
                                    new_word.word = new_word.translation;
                                }
                                break;
                            }
                            case TargetModification::MOD_DELETE: {
                                if (!mod.text.empty()) {
                                    if (new_word.translation.size() >= mod.text.size() &&
                                        new_word.translation.substr(new_word.translation.size() - mod.text.size()) == mod.text) {
                                        new_word.translation = new_word.translation.substr(0, new_word.translation.size() - mod.text.size());
                                        new_word.word = new_word.translation;
                                    }
                                }
                                break;
                            }
                            default:
                                break;
                        }
                        reordered_arr.push_back(new_word);
                        found = true;
                        break;
                    }
                    
                }
                else {
              
                    Word new_word;
                    new_word.word = target_token;
                    new_word.translation = target_token;
                    new_word.type = -1;
                    new_word.flags = 0;
                    new_word.trigger_word = "";
                    reordered_arr.push_back(new_word);
                }
            }
            
            size_t total_consumed = (captured_count > 0) ? captured_indices.size() : pattern_len;
         
            for (size_t j = 0; j < total_consumed; ++j) {
                consumed[i + j] = true;
            }
            
            matched = true;
            i += total_consumed;
            break;
        }
        
        if (!matched) {
            reordered_arr.push_back(sentence_arr[i]);
            ++i;
        }
    }
    
    for (size_t idx = 0; idx < reordered_arr.size(); ++idx) {
     
    }
    
    return reordered_arr;
}

// all the lookups
using Reorder = Vector<Word>(*)(const Vector<Word>&);
using NounLookup = Word(*)(const String&, const Vector<String>, int);

//ngram groups
inline String unigramLookup(Vector<String>& array_of_words,
                            const Vector<int>& ignore_flags, Reorder reorder_helpers = nullptr, NounLookup nounLookup = nullptr, bool non_spaced = false){

    Vector<Word> sentence_arr;
    Vector<Word> word_arr;

    int match_type;
    String sentence;
    
    for(size_t i = 0; i < array_of_words.size(); ++i){
        if (array_of_words[i].find(PREV_MARKER) != String::npos) {
            Word match_ = {array_of_words[i], array_of_words[i], -1, 0, 0, ""};
            sentence_arr.push_back({array_of_words[i], array_of_words[i], -1, 0, 0, ""});
            word_arr.push_back(match_);
            continue;
        }
        
        Word match = nounLookup(array_of_words[i], array_of_words, i);

        array_of_words[i] = match.word; 
        
        switch (ignore_flags[i]) {
            case 0: {
                 match_type = match.type;
                if(match.type == -1) match_type = 0;
                String token = array_of_words[i];
                bool isPunct = (token.size() == 1 && (token[0] == '.' || token[0] == '!' || 
                                token[0] == '?' || token[0] == ',' || token[0] == ':' ||
                                token[0] == ';' || token[0] == '(' || token[0] == ')'));
                if (isPunct) {
                    match_type = -1;
                }
                Word match_ = match;
                match_.type = match_type;
                sentence_arr.push_back(match_);
                word_arr.push_back(match_);
                break;
            }
            case 1: {
                Word match_ = {array_of_words[i], array_of_words[i], -1, 0, 0, ""};
                sentence_arr.push_back({array_of_words[i], array_of_words[i], -1, 0, 0, ""});
                word_arr.push_back(match_);
                break;
            }
            default:
                 break;
        }
    }
    
    if(word_arr.size() > 0) {
  
        if(transfers.size() > 0) sentence_arr = applyTransferRules(word_arr);
        sentence_arr = reorder_helpers(sentence_arr);
    }
    
    for (size_t i = 0; i < sentence_arr.size(); ++i) {
        const String& token = sentence_arr.at(i).translation;
        char firstChar = token.empty() ? '\0' : token[0];
        bool isPunctuation = (firstChar == '?' || firstChar == '!' || 
                              firstChar == '.' || firstChar == ',' ||
                              firstChar == '-' || firstChar == '/' || firstChar == ':');
        if (!sentence.empty() && !isPunctuation && !non_spaced) {
            sentence += " ";
        }
        sentence += token;
    }
    return sentence;
}

template <size_t N>
inline String bigramLookup(const Entry (&fixed_ngrams)[N],
                                const Vector<String>& words,
                                Vector<int>& ignore_flags, Reorder reorder_helpers = nullptr, NounLookup nounLookup = nullptr, bool non_spaced = false) {
    Vector<String> mended_array_of_words;
    Vector<int> new_ignore_flags;

    size_t i = 0;
    while (i < words.size()) {
        if (i + 1 < words.size() && ignore_flags[i] == 0 && ignore_flags[i + 1] == 0) {
            String bigram;
            bigram += words[i];
            bigram += "_";
            bigram += words[i + 1];
            const char* bigram_translation = lookup(fixed_ngrams, bigram.c_str());
            
            if (bigram_translation) {
                mended_array_of_words.push_back(bigram_translation);
                new_ignore_flags.push_back(1);  
                i += 2;  
                continue;
            }
        }
        
        mended_array_of_words.push_back(words[i]);
        new_ignore_flags.push_back(ignore_flags[i]);
        i++;
    }

    return unigramLookup(mended_array_of_words, new_ignore_flags, reorder_helpers, nounLookup, non_spaced);
}


template <size_t N>
inline String trigramLookup(const Entry (&fixed_ngrams)[N],
                                 const Vector<String>& words, Reorder reorder_helpers = nullptr, NounLookup nounLookup = nullptr, bool non_spaced = false) { 
                                    Vector<String> mended;
                                    Vector<int> ignore_flags(words.size(), 0);

    size_t i = 0;
    while (i < words.size()) {
        if (i + 2 < words.size()) {
            String trigram;
            trigram += words[i];
            trigram += "_";
            trigram += words[i + 1];
            trigram += "_";
            trigram += words[i + 2];
            
            const char* trigram_translation = lookup(fixed_ngrams, trigram.c_str());
            
            if (trigram_translation) {
                mended.push_back(trigram_translation);
                ignore_flags.push_back(1);  
                i += 3;  
                continue;
            }
        }
        mended.push_back(words[i]);
        ignore_flags.push_back(0);
        i++;
    }

    // Then process bigrams on the result
   return bigramLookup(fixed_ngrams, mended, ignore_flags, reorder_helpers, nounLookup, non_spaced);
}







inline void to_lower(char *s) {
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] - 'A' + 'a';
        }
    }
}


inline Vector<String> tokenize(const String &text) {
      Vector<String> tokens;
      String current;
      size_t i = 0;

      while (i < text.size()) {
          unsigned char c = text[i];

          if ((c & 0x80) == 0) {
              if (std::isalnum(c)) {
                  current += c;
              } else {
                  if (!current.empty()) {
                      tokens.push_back(current);
                      current.clear();
                  }
                  if (!std::isspace(c))
                      tokens.push_back(String(1, c));
              }
              ++i;
          } else {
              size_t len = 0;
              if ((c & 0xE0) == 0xC0) len = 2;
              else if ((c & 0xF0) == 0xE0) len = 3;
              else if ((c & 0xF8) == 0xF0) len = 4;
              else len = 1;

              String utf8char = text.substr(i, len);
              current += utf8char;
              i += len;
          }
      }

      if (!current.empty())
          tokens.push_back(current);
      return tokens;
  }

  inline Vector<String> tokenize_cjk(const String &text) {
    Vector<String> tokens;
    size_t i = 0;

    while (i < text.size()) {
        unsigned char c = text[i];

        if ((c & 0x80) == 0) {
            // ASCII alphanumeric word
            if (std::isalnum(c)) {
                String current;
                while (i < text.size() && (text[i] & 0x80) == 0 && std::isalnum(text[i])) {
                    current += text[i];
                    ++i;
                }
                tokens.push_back(current);
            } else {
                // ASCII punctuation or space
                if (!std::isspace(c))
                    tokens.push_back(String(1, c));
                ++i;
            }
        } else {
            // Multi‑byte UTF‑8 character – treat as one token
            size_t len = 0;
            if ((c & 0xE0) == 0xC0) len = 2;
            else if ((c & 0xF0) == 0xE0) len = 3;
            else if ((c & 0xF8) == 0xF0) len = 4;
            else len = 1; // fallback

            tokens.push_back(text.substr(i, len));
            i += len;
        }
    }

    return tokens;
}

StressRule stress_rules[] = {
    {STRESS_LAST, STRESS_LAST}  // Always stress last syllable
};

Vector<String> ipa_vowels = {
    "a", "e", "i", "o", "u", 
    "æ", "ɯ", "ə", "ɑ", "ɔ", "ɛ", "ɪ", "ʊ",
    "ã", "ẽ", "ĩ", "õ", "ũ", 
    "ä", "ë", "ï", "ö", "ü"
};

// remember to wrap this around a flag to compile without or without IPA output

bool rule_applies(const IpaRules& rule, const String& sentence, size_t i) {
    size_t ruleLen = rule.rule.string.length();
    size_t sentLen = sentence.length();
    
    if (i + ruleLen > sentLen) return false;
    if (sentence.substr(i, ruleLen) != rule.rule.string) return false;
    
    switch(rule.position) {
        case ALWAYS:
            return true;
        case BEGINNING:
            return (i == 0);
        case ENDING:
            return (i + ruleLen == sentLen);
        case ISOLATED:
            return (i == 0 && i + ruleLen == sentLen);
        case BEFORE: {
            size_t contextPos = i + ruleLen;
            if (contextPos + rule.with.length() > sentLen) return false;
            return sentence.substr(contextPos, rule.with.length()) == rule.with;
        }
        case AFTER: {
            if (i < rule.with.length()) return false;
            size_t contextPos = i - rule.with.length();
            return sentence.substr(contextPos, rule.with.length()) == rule.with;
        }
        case BETWEEN: {
            if (rule.with.length() != 2) return false;
            String before = rule.with.substr(0, 1);
            String after = rule.with.substr(1, 1);
            bool beforeMatches = (i >= before.length() && 
                sentence.substr(i - before.length(), before.length()) == before);
            bool afterMatches = (i + ruleLen + after.length() <= sentLen &&
                sentence.substr(i + ruleLen, after.length()) == after);
            return beforeMatches && afterMatches;
        }
        default:
            return false;
    }
}

void sort_rules(IpaRules rules[], size_t count) {
    for (size_t i = 0; i < count - 1; i++) {
        for (size_t j = 0; j < count - i - 1; j++) {
            bool swap_needed = false;
            
            if (rules[j].rule.string.length() < rules[j + 1].rule.string.length()) {
                swap_needed = true;
            }
            else if (rules[j].rule.string.length() == rules[j + 1].rule.string.length()) {
                if (rules[j].position == ALWAYS && rules[j + 1].position != ALWAYS) {
                    swap_needed = true;
                }
                else if (rules[j].position != ALWAYS && rules[j + 1].position == ALWAYS) {
                    swap_needed = false;
                }
                else if (rules[j].position < rules[j + 1].position) {
                    swap_needed = true;
                }
            }
            
            if (swap_needed) {
                IpaRules temp = rules[j];
                rules[j] = rules[j + 1];
                rules[j + 1] = temp;
            }
        }
    }
}


bool is_vowel_ipa(const String& ch) {
    for (size_t i = 0; i < ipa_vowels.size(); i++) {
        if (ipa_vowels[i] == ch) return true;
    }
    return false;
}

size_t utf8_len(unsigned char c) {
    if ((c & 0x80) == 0) return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 1;
}

int count_syllables(const String& word) {
    int count = 0;
    bool in_vowel = false;
    size_t i = 0;
    
    while (i < word.length()) {
        size_t len = utf8_len(word[i]);
        String ch = word.substr(i, len);
        
        if (ch == "ˈ" || ch == "ˌ" || ch == "ʔ") {
            i += len;
            continue;
        }
        
        if (is_vowel_ipa(ch)) {
            if (!in_vowel) {
                count++;
                in_vowel = true;
            }
        } else {
            in_vowel = false;
        }
        
        i += len;
    }
    
    return count;

}

String apply_stress(const String& word) {
    Vector<size_t> syllable_starts;
    size_t i = 0;
    bool in_vowel = false;
    size_t last_vowel_end = 0;  

    while (i < word.length()) {
        size_t len = utf8_len(word[i]);
        String ch = word.substr(i, len);

        if (ch == "ˈ" || ch == "ˌ" || ch == "ʔ") {
            i += len;
            continue;
        }

        if (is_vowel_ipa(ch)) {
            if (!in_vowel) {
                syllable_starts.push_back(last_vowel_end);
                in_vowel = true;
            }
            i += len;
        } else {
            if (in_vowel) {
                last_vowel_end = i;
                in_vowel = false;
            }
            i += len;
        }
    }

    int syl_count = (int)syllable_starts.size();
    if (syl_count <= 1) return word;  

    int target_syl = 0;
    size_t num_stress_rules = sizeof(stress_rules) / sizeof(stress_rules[0]);
    for (size_t r = 0; r < num_stress_rules; r++) {
        switch (stress_rules[r].position) {
            case STRESS_LAST:         target_syl = syl_count - 1; break;
            case STRESS_PENULT:       target_syl = (syl_count >= 2) ? syl_count - 2 : 0; break;
            case STRESS_ANTEPENULT:   target_syl = (syl_count >= 3) ? syl_count - 3 : 0; break;
            case STRESS_FIRST:        target_syl = 0; break;
            case STRESS_SECOND:       target_syl = (syl_count >= 2) ? 1 : 0; break;
            case STRESS_NONE:         return word;
        }
        break;
    }

    if (target_syl < 0 || target_syl >= syl_count) target_syl = syl_count - 1;

    size_t target_pos = syllable_starts[target_syl];
    String result = "";
    i = 0;
    while (i < word.length()) {
        if (i == target_pos) result += "ˈ";
        size_t len = utf8_len(word[i]);
        result += word.substr(i, len);
        i += len;
    }
    return result;
}

String get_ipa(const String& sentence) {
    String ipa_output = "";
    size_t num_rules = sizeof(ipa_rules) / sizeof(ipa_rules[0]);
    
    static bool sorted = false;
    if (!sorted) {
        sort_rules(ipa_rules, num_rules);
        sorted = true;
    }
    
    Vector<String> tokens = tokenize(sentence);
    
    for (size_t t = 0; t < tokens.size(); t++) {
        String word = tokens[t];
        String processed_word = "";
        size_t i = 0;
        
        while (i < word.length()) {
            bool matched = false;
            for (size_t j = 0; j < num_rules; j++) {
                if (rule_applies(ipa_rules[j], word, i)) {
                    processed_word += ipa_rules[j].rule.sound;
                    i += ipa_rules[j].rule.string.length();
                    matched = true;
                    break;
                }
            }
            if (!matched) {
                processed_word += word[i];
                i++;
            }
        }
        
        processed_word = apply_stress(processed_word);
        
        if (t > 0) ipa_output += " ";
        ipa_output += processed_word;
    }
    
    return ipa_output;
}




inline uint32_t next_utf8_char(const String& s, size_t& i) {
    unsigned char c = s[i];
    uint32_t cp = 0;
    size_t extra = 0;

    if (c <= 0x7F) {
        cp = c;
        extra = 0;
    } else if ((c & 0xE0) == 0xC0) { 
        cp = c & 0x1F;
        extra = 1;
    } else if ((c & 0xF0) == 0xE0) { 
        cp = c & 0x0F;
        extra = 2;
    } else if ((c & 0xF8) == 0xF0) {
        cp = c & 0x07;
        extra = 3;
    } else {
        i++; 
        return 0xFFFD; 
    }

    for (size_t j = 1; j <= extra; ++j) {
        if (i + j >= s.size()) return 0xFFFD;
        unsigned char cc = s[i + j];
        if ((cc & 0xC0) != 0x80) return 0xFFFD;
        cp = (cp << 6) | (cc & 0x3F);
    }

    i += extra;
    return cp;
}

struct Action {
    const char* key;
    void (*fp)(Vector<Word>&, const Word&, const Word&, const char*);  
};


inline Action actions[] = {
    {"INVERT", &invert },
    {"REMOVE_FIRST", &remove_first},
    {"SANDWICH", &sandwich},
    {"REPLACE_FIRST", &replace_first}
};


inline Vector<String> parser(const String& s) {
    Vector<String> out;
    String current;

    for (char c : s) {
        if (c == ' ') {
            if (!current.empty()) {
                out.push_back(current);
                current.clear();
            }
        } else {
            current.push_back(c);
        }
    }

    if (!current.empty()) {
        out.push_back(current);
    }

    return out;
}

inline auto lookupFunction(const char* query)
    -> void (*)(Vector<Word>&, const Word&, const Word&, const char*)  
{
    int actionCount = sizeof(actions) / sizeof(actions[0]);

    for (int i = 0; i < actionCount; i++) {

        const char* p = query;
        if (!p) continue;          
        const char* r = actions[i].key;
        while (*p && *r && (*p == *r)) {
            p++;
            r++;
        }
        if (*p == *r) {
            return actions[i].fp;
        }
    }

    return nullptr;



}



inline bool default_multibyte = false;
inline bool default_autoCorrect = false;
inline unsigned int ngrams_length = 0;
static inline Entry default_fixed_ngrams[5];

Info default_info = {
    SVO,
    SOV, 
      // genitive
    { 
        { MIDDLE_WORD, 2, POSSESSED_FIRST, INDEFINITE, "do" },
        { MIDDLE_WORD, 2, POSSESSED_FIRST, INDEFINITE, "da" },
    },
    { 
        { MIDDLE_WORD, 0, OWNER_FIRST, INDEFINITE, "yõg" },
    },
    1,                
    1,                
    NOUN_FIRST,        
    NOUN_FIRST,
    // definiteness
    { 
        
       { PREV_WORD, "uma", 0, NO_VOWEL_HARMONY },
        { PREV_WORD, "as", 0, NO_VOWEL_HARMONY },
        { PREV_WORD, "os", 0, NO_VOWEL_HARMONY },
       { PREV_WORD, "um", 0, NO_VOWEL_HARMONY },
       { PREV_WORD, "o", 0, NO_VOWEL_HARMONY },
       { PREV_WORD, "a", 0, NO_VOWEL_HARMONY },
    },
    { 
        { SUFFIX, "", 0, NO_VOWEL_HARMONY }
    },
    6,
    1,
    {0},
    {1}
};


inline unsigned int nouns_length = 0;
inline Entry default_nouns[MAX_ENTRIES];

inline unsigned int verbs_length = 0;
inline Verb default_verbs[MAX_ENTRIES];

inline Vector<AtomString> temp_verb_endings;
inline String temp_required_ending;
inline String temp_affix;
inline uint8_t temp_type = 0;

uint64_t temp_morph_flag = 0;
AtomString temp_morph_ending;
AtomString temp_morph_form;
int temp_morph_type = 0;
int temp_morph_morphology = 0;
int temp_morph_result_type = 0;
Vector<int> temp_morph_apply_only_to;
int temp_morph_stackable = 0;
int temp_morph_agreement = 1;

// whenever parsing the binary, use push_back instead of [i] = value to add new entries.
// unless etl?

inline VerbRuleDictionary default_endings;

inline VerbConjugationDictionary default_conjugations;

inline Morph default_morph_from;
inline Morph default_morph_to;

CASE_DEF(cases,
{
    {NOMINATIVE, 0, "", " te", nullptr, SUFFIX}
});

HOMONYM_DEF(
    are,
    HOMONYM_OUTCOMES(
        { "são", VERB },
        { "somos", VERB }
    ),
    HOMONYM_FORBIDDEN(98, 99),
    "eles","$", "nós"
);


Homonym default_homonyms[] = {
    HOMONYM("are", are)
};



inline unsigned int adjective_length = 0;
inline Entry default_adjectives[MAX_ENTRIES];

inline unsigned int pronoun_length = 0;
inline Entry default_pronouns[MAX_ENTRIES];


inline unsigned int adverb_length = 0;
inline Entry default_adverbs[MAX_ENTRIES];

inline char buffer[250];

//default for binary modules


struct NormalizationRule {
     String from;
     String to;
     int type;
};
inline unsigned int target_flags_length = 0;
inline unsigned int normalizationRuleLength = 0;
inline NormalizationRule default_normalizationRules[MAX_NORMALIZATION_RULES];
inline unsigned int normalization_ruleCount = 0;

inline String normalize(String word) {
    String normalized_ = word;
    for (unsigned int i = 0; i < normalizationRuleLength; ++i) { 
        NORMALIZE(
            default_normalizationRules[i].from,
            default_normalizationRules[i].type,
            default_normalizationRules[i].to
        );
    }
    return normalized_;
}


// this is a helper function to find the other forms of words based on different flags, given correct definition
// imagine we need to find the oblique version of the pronoun "he" -> "him".
// in the dictionary, both entries will share the pronoun type and the 3rd person and masculine flags. 
// 'him' will have the oblique flag so we do ("him", OBLLIQUE) to find it 
String find_relation(Word word, uint64_t requirement) {
    uint64_t required_flags = word.flags | requirement;
    
    // Search pronoun dictionary for an entry with matching flags
    for (size_t i = 0; i < pronoun_length; ++i) {
        const Entry& entry = default_pronouns[i];
        
        // Check if this entry has ALL required flags
        if ((entry.flags & required_flags) == required_flags) {
            return entry.t ? entry.t : "";
        }
    }
    return "";
}
static Vector<Word> default_reorderHelpers(const Vector<Word>& copy){
    Vector<Word> sentence_arr = copy;
    Vector<Word> reordered_arr;
    Vector<int> skip_indices;
    const String PREV_MARKER = "\u200B";

    for (size_t i = 0; i < sentence_arr.size(); ++i) {
        INIT_REORDER()
        DEFAULT()
    }
    CLEANUP(reordered_arr)

    HANDLE_TRANSFERS(&default_info, reordered_arr, default_morph_to);
    HANDLE_CASE(&default_info, NO_CASE, &cases);

    // Preserve the verb if it was removed
    bool has_verb = false;
    for (size_t i = 0; i < reordered_arr.size(); ++i) {
        if (reordered_arr[i].type == VERB && !reordered_arr[i].translation.empty()) {
            has_verb = true;
            break;
        }
    }
    if (!has_verb) {
        for (size_t i = 0; i < sentence_arr.size(); ++i) {
            if (sentence_arr[i].type == VERB && !sentence_arr[i].translation.empty()) {
                bool already = false;
                for (size_t j = 0; j < reordered_arr.size(); ++j) {
                    if (reordered_arr[j].word == sentence_arr[i].word) {
                        already = true;
                        break;
                    }
                }
                if (!already) {
                    reordered_arr.push_back(sentence_arr[i]);
                    break;
                }
            }
        }
    }

    // Marker-based skip
    for (size_t i = 0; i < reordered_arr.size(); ++i) {
        size_t pos = reordered_arr[i].word.find(PREV_MARKER);
        if (pos != String::npos) {
            reordered_arr[i].word = reordered_arr[i].word.substr(0, pos);
            if (i > 0) skip_indices.push_back(i - 1);
        }
    }

    // trigger_word-based skip
    for (size_t i = 0; i < reordered_arr.size(); ++i) {
        if (!reordered_arr[i].trigger_word.empty()) {
            for (size_t j = 0; j < reordered_arr.size(); ++j) {
                if (reordered_arr[j].word == reordered_arr[i].trigger_word) {
                    skip_indices.push_back(j);
                    break;
                }
            }
        }
    }

    Vector<Word> final_arr;
    
    // FIRST: apply all skips to get the final word array
    for (size_t i = 0; i < reordered_arr.size(); ++i) {
        bool should_skip = false;
        for (int idx : skip_indices) {
            if (idx == (int)i) { should_skip = true; break; }
        }
        if (!should_skip) final_arr.push_back(reordered_arr[i]);
    }

    // THEN: apply morph rules on the final filtered array
    final_arr = applyMorphRules(final_arr);

    
    return final_arr;
}
static Word default_verb_lookup(const String& word, const Vector<String> sentence, int word_index) {
  
    static bool sanitized = false;
    if (!sanitized) {
        for (auto& conj : default_conjugations) {
            if (conj.vowel_harmony != nullptr) {
                uintptr_t addr = reinterpret_cast<uintptr_t>(conj.vowel_harmony);
                if (addr < 0x1000 || addr > 0x7fffffffffff || 
                    (addr >= 0x7fff00000000 && addr <= 0x7fffffffffff)) {
                    conj.vowel_harmony = nullptr;
                }
            }
        }
        sanitized = true;
    }
    
    String marked_word = word;
    bool prev_word_matched = false;
    String prev_word_trigger = "";
    int prev_word_morphology = -1;
    
    if (word_index < 0 || word_index >= (int)sentence.size()) {
        return Word{word, word, -1, 0, 0};
    }

    if (verbs_length == 0) {
        return Word{word, word, -1, 0, 0};
    }

    bool multibyte = default_multibyte;

    Verb v = find_verb_in_array(default_verbs, verbs_length, word.c_str());
    if (v.t && *v.t) {
        uint64_t verb_flags = v.flags;
        uint64_t flags = verb_flags;
        return Word{word, normalize(v.t), VERB, 0, flags};
    }
 
    if (!default_morph_from.variations.empty()) {
        const Morph* morph_from_ptr = &(default_morph_from);
        const Morph* morph_to_ptr = &(default_morph_to);
        String current_word = word;
        Vector<const MorphVariation*> applied_morphs;
        bool found_root = false;
        const char* result = nullptr;
        uint64_t flags = 0;
        int safety = 10;
        
        while (!found_root && safety-- > 0) {
            MorphResult p = detect_morph(current_word, morph_from_ptr, sentence, word_index);
            
            if (p.matched_variation != nullptr) {
                if (p.matched_variation->type == PREV_WORD) {
                    prev_word_matched = true;
                    prev_word_morphology = p.matched_variation->morphology;
                    if (word_index > 0 && word_index < (int)sentence.size()) {
                        prev_word_trigger = sentence[word_index - 1];
                    }
                }
                
                marked_word = p.root;
                if (p.matched_variation->stackable == STACKABLE) {
                    applied_morphs.push_back(p.matched_variation);
                    current_word = p.root;
                    
                    Verb v = find_verb_in_array(default_verbs, verbs_length, current_word.c_str());
                    if (v.t && *v.t) {
                        result = v.t;
                        flags = v.flags;
                        found_root = true;
                        break;
                    } 
                } else {
                    applied_morphs.push_back(p.matched_variation);
                    Verb v = find_verb_in_array(default_verbs, verbs_length, p.root.c_str());
                    if (v.t && *v.t) {
                        result = v.t;
                        flags = v.flags;
                        found_root = true;
                    } 
                    break;
                }
            } else {
                break;
            }
        }
        
        if (found_root && !applied_morphs.empty()) {
            String translation = result;
            const Morph* morph_to_ptr = &(default_morph_to);
            for (auto it = applied_morphs.rbegin(); it != applied_morphs.rend(); ++it) {
                translation = apply_morph(translation, *it, morph_to_ptr, flags);
                for (const auto& target_var : morph_to_ptr->variations) {
                    if (target_var.morphology == (*it)->morphology) {
                        flags |= target_var.required_flags;
                        break;
                    }
                }
            }
            Word result_word;
            result_word.word = marked_word;
            result_word.translation = normalize(translation);
            result_word.type = VERB;
            result_word.flags = flags;
            if (prev_word_matched && !prev_word_trigger.empty()) {
                result_word.trigger_word = prev_word_trigger;
            }
            return result_word;
        }
    }

    // Try suffix-stripping using verb endings
    if (!default_endings.empty() && !default_conjugations.empty()) {
        if (default_verbs == nullptr || verbs_length == 0) {
            return Word{word, word, -1, 0, 0};
        }
        for (const auto& group : default_endings) {
            for (const auto& ending : group.endings) {
                int ending_form = group.form;
                int ending_type = group.type;
                
                if (ending_type == TRANSFIX) {
                    if (default_verbs == nullptr || verbs_length == 0) {
                        continue;
                    }
                    for (const auto& dict_entry : default_verbs) {
                        String dict_pattern = dict_entry.w;
                        String template_pattern = ending;
                        WordString reconstructed = "";
                        size_t dict_pos = 0;
                        size_t temp_pos = 0;
                        while (temp_pos < template_pattern.size() && dict_pos < dict_pattern.size()) {
                            if (template_pattern[temp_pos] == '_') {
                                while (dict_pos < dict_pattern.size() && dict_pattern[dict_pos] != '_') {
                                    reconstructed += dict_pattern[dict_pos];
                                    dict_pos++;
                                }
                                if (dict_pos < dict_pattern.size() && dict_pattern[dict_pos] == '_') {
                                    dict_pos++;
                                }
                                temp_pos++;
                            } else {
                                while (dict_pos < dict_pattern.size() && dict_pattern[dict_pos] != '_') {
                                    reconstructed += dict_pattern[dict_pos];
                                    dict_pos++;
                                }
                                if (dict_pos < dict_pattern.size() && dict_pattern[dict_pos] == '_') {
                                    reconstructed += template_pattern[temp_pos];
                                    dict_pos++;
                                } else {
                                    reconstructed += template_pattern[temp_pos];
                                }
                                temp_pos++;
                            }
                        }
                        while (temp_pos < template_pattern.size()) {
                            if (template_pattern[temp_pos] != '_') {
                                reconstructed += template_pattern[temp_pos];
                            }
                            temp_pos++;
                        }
                        while (dict_pos < dict_pattern.size()) {
                            if (dict_pattern[dict_pos] != '_') {
                                reconstructed += dict_pattern[dict_pos];
                            }
                            dict_pos++;
                        }
                        if (reconstructed == word) {
                            Verb v = find_verb_in_array(default_verbs, verbs_length, dict_pattern.c_str());
                            if (v.t && *v.t) {
                                String translation = v.t;
                                uint64_t verb_flags = v.flags;
                                const Morph* morph_to_ptr = &(default_morph_to);
                                MorphVariation temp_var;
                                temp_var.type = ending_type;
                                temp_var.ending = ending;
                                temp_var.form = "";
                                temp_var.flag = 0;
                                temp_var.morphology = 0;
                                temp_var.result_type = VERB;
                                temp_var.stackable = STACKABLE;
                                temp_var.vowel_harmony = nullptr;
                                temp_var.agreement = 0;
                                translation = apply_morph(translation, &temp_var, morph_to_ptr, verb_flags);
                                for (const auto& target_var : morph_to_ptr->variations) {
                                    if (target_var.morphology == temp_var.morphology) {
                                        verb_flags |= target_var.required_flags;
                                        break;
                                    }
                                }
                                return Word{word, normalize(translation), VERB, 0, verb_flags};
                            }
                        }
                    }
                    continue;
                }
                
                if (word.size() <= ending.size()) {
                    continue;
                }
                
                bool ends_with = false;
                if (multibyte) {
                    if (word.compare(word.size() - ending.size(), ending.size(), ending) == 0)
                        ends_with = true;
                } else {
                    if (word.compare(word.size() - ending.size(), ending.size(), ending) == 0)
                        ends_with = true;
                }
                if (!ends_with) {
                    continue;
                }

                String root = word.substr(0, word.size() - ending.size());
               
                Verb v = find_verb_in_array(default_verbs, verbs_length, root.c_str());
                if (v.t && *v.t) {
                    String translation = v.t;
                    uint64_t verb_flags = v.flags;
                    String affix = "";
                    String result = translation;
                    bool conjugation_applied = false;

                    for (const auto& conj : default_conjugations) {
                        if (conj.form != ending_form) continue;
                        bool condition_met = conj.required_ending.empty();
                        if (!conj.required_ending.empty()) {
                            if (translation.size() >= conj.required_ending.size() &&
                                translation.compare(translation.size() - conj.required_ending.size(),
                                                    conj.required_ending.size(),
                                                    conj.required_ending) == 0) {
                                condition_met = true;
                            }
                        }

                        if (condition_met) {
                            String stem = translation;
                            if (conj.vowel_harmony != nullptr) {
                                const HarmonyTable& v_h = *(conj.vowel_harmony);
                                affix = checkVowelHarmony(stem, conj.affix, v_h);
                            } else {
                                affix = conj.affix;
                            }
                            if (!conj.required_ending.empty()) {
                                stem = translation.substr(0, translation.size() - conj.required_ending.size());
                            }
                            if (conj.type == PREFIX) {
                                result = affix + translation;
                            } else if (conj.type == PREV_WORD) {
                                if (!affix.empty() && affix.back() != ' ') {
                                    result = affix + " " + translation;
                                }
                            } else if (conj.type == SUFFIX) {
                                result = translation + affix;
                            } else if (conj.type == NEXT_WORD) {
                                if (!affix.empty() && affix.front() != ' ') {
                                    result = translation + " " + affix;
                                }
                            }
                            translation = stem;
                            conjugation_applied = true;
                            break;
                        }
                    }

                    if (conjugation_applied) {
                        Word result_word;
                        result_word.word = marked_word;
                        result_word.translation = normalize(result);
                        result_word.type = VERB;
                        result_word.flags = verb_flags;
                        if (prev_word_matched && !prev_word_trigger.empty()) {
                            result_word.trigger_word = prev_word_trigger;
                            // Add required_flags from PREV_WORD target morphology
                            const Morph* morph_from_ptr = &(default_morph_from);
                            const Morph* morph_to_ptr = &(default_morph_to);
                            for (const auto& var : morph_from_ptr->variations) {
                                if (var.type == PREV_WORD && var.morphology == prev_word_morphology) {
                                    for (const auto& target_var : morph_to_ptr->variations) {
                                        if (target_var.morphology == var.morphology) {
                                            result_word.flags |= target_var.required_flags;
                                            break;
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                        return result_word;
                    }
                }
            }
        }
    } 
    return Word{word, word, -1, 0, 0};
}
static Word default_nounLookup(const String& word, const Vector<String> sentence, int word_index) {

    struct LookupConfig {
        const Entry* dict;
        unsigned int length;
        int type;
    };
    const LookupConfig configs[] = {
        {default_nouns, nouns_length, NOUN}
    };
    if (word_index < 0 || word_index >= (int)sentence.size()) {
        return Word{word, word, -1, 0, 0};
    }

 /* PREV_WORD DETECTION (only for nouns, AFTER stacking) */
{
    bool is_verb = false;

    bool is_noun = false;
    for (const auto& cfg : configs) {
        if (lookup_test(cfg.dict, cfg.length, word.c_str())) {
            is_noun = true;
            break;
        }
    }

    if (!is_noun) {
        Verb v = find_verb_in_array(default_verbs, verbs_length, word.c_str());
        if (v.t && *v.t) {
            is_verb = true;
        }
        
        if (!is_verb && verbs_length > 0 && !default_endings.empty()) {
            for (const auto& group : default_endings) {
                for (const auto& ending : group.endings) {
                    if (word.size() > ending.size() && 
                        word.compare(word.size() - ending.size(), ending.size(), ending) == 0) {
                        String root = word.substr(0, word.size() - ending.size());
                        Verb v2 = find_verb_in_array(default_verbs, verbs_length, root.c_str());
                        if (v2.t && *v2.t) {
                            is_verb = true;
                            break;
                        }
                    }
                }
                if (is_verb) break;
            }
        }
    }
    
    if (!is_verb) {
        const Morph* morph_from_ptr = &(default_morph_from);
        for (const auto& var : morph_from_ptr->variations) {
            if (var.type == PREV_WORD) {
                if (word_index > 0 && word_index < (int)sentence.size() && 
                    sentence[word_index - 1] == var.ending) {
                    const char* trans = lookup_test(default_nouns, nouns_length, word.c_str());
                    if (trans) {
                        String translation = trans;
                        uint64_t word_flags = lookupFlags_test(default_nouns, nouns_length, word.c_str());
                        const Morph* morph_to_ptr = &(default_morph_to);
for (const auto& target_var : morph_to_ptr->variations) {
    if (target_var.morphology == var.morphology) {
        
        translation = apply_morph(translation, &target_var, morph_to_ptr, word_flags);
        word_flags |= target_var.required_flags;
        break;
    }
    
}

                        Word result_word;
                        result_word.word = word + PREV_MARKER;
                        result_word.trigger_word = sentence[word_index - 1];
                        result_word.translation = normalize(translation);
                        result_word.type = NOUN;
                        result_word.flags = word_flags;
                        return result_word;
                    }
                }

                
            }
        }
    }
}

    /* REGULAR MORPHOLOGICAL DETECTION (stackable suffixes) */
    {
        const Morph* morph_from_ptr = &(default_morph_from);
        String current_word = word;
        Vector<const MorphVariation*> applied_morphs;
        bool found_root = false;
        const char* result = nullptr;
        uint64_t flags = 0;
        int found_type = UNKNOWN;
        int safety = 30;

        while (!found_root && safety-- > 0) {
            bool non_stackable_matched = false;
            for (const auto& var : morph_from_ptr->variations) {
                if (var.type != SUFFIX || var.stackable) continue;
                const String& ending = var.ending;
                const String& trigger = var.form;
                if (ending.empty() || current_word.size() <= ending.size()) continue;
                if (current_word.compare(current_word.size() - ending.size(), ending.size(), ending) != 0) continue;

                String base = current_word.substr(0, current_word.size() - ending.size());
                String root = base + trigger;

                bool root_exists = false;
                for (const auto& cfg : configs) {
                    if (lookup_test(cfg.dict, cfg.length, root.c_str())) {
                        root_exists = true;
                        break;
                    }
                }

                if (root_exists) {
                    applied_morphs.push_back(&var);
                    current_word = root;
                    found_root = true;
                    non_stackable_matched = true;
                    for (const auto& cfg : configs) {
                        const char* lookup_result = lookup_test(cfg.dict, cfg.length, root.c_str());
                        if (lookup_result) {
                            result = lookup_result;
                            flags = lookupFlags_test(cfg.dict, cfg.length, root.c_str());
                            found_type = cfg.type;
                            break;
                        }
                    }
                    break;
                }
            }
            if (non_stackable_matched) break;

            Vector<const MorphVariation*> matching_morphs;
            for (const auto& var : morph_from_ptr->variations) {
                if (var.type != SUFFIX || !var.stackable) continue;
                const String& ending = var.ending;
                const String& trigger = var.form;
                if (ending.empty() || current_word.size() <= ending.size()) continue;
                if (current_word.compare(current_word.size() - ending.size(), ending.size(), ending) != 0) continue;
                matching_morphs.push_back(&var);
            }

            bool stripped = false;
            for (const auto& var : matching_morphs) {
                String base = current_word.substr(0, current_word.size() - var->ending.size());
                String root = base + var->form;

                bool root_exists = false;
                for (const auto& cfg : configs) {
                    if (lookup_test(cfg.dict, cfg.length, root.c_str())) {
                        root_exists = true;
                        break;
                    }
                }

                if (root_exists) {
                    applied_morphs.push_back(var);
                    current_word = root;
                    stripped = true;
                    found_root = true;
                    for (const auto& cfg : configs) {
                        const char* lookup_result = lookup_test(cfg.dict, cfg.length, root.c_str());
                        if (lookup_result) {
                            result = lookup_result;
                            flags = lookupFlags_test(cfg.dict, cfg.length, root.c_str());
                            found_type = cfg.type;
                            break;
                        }
                    }
                    break;
                }
            }

            if (found_root) break;

            if (!stripped && !matching_morphs.empty()) {
                const auto& var = matching_morphs[0];
                String base = current_word.substr(0, current_word.size() - var->ending.size());
                String root = base + var->form;
                applied_morphs.push_back(var);
                current_word = root;
                stripped = true;
            }

            if (!stripped) {
                for (const auto& cfg : configs) {
                    const char* lookup_result = lookup_test(cfg.dict, cfg.length, current_word.c_str());
                    if (lookup_result) {
                        result = lookup_result;
                        flags = lookupFlags_test(cfg.dict, cfg.length, current_word.c_str());
                        found_type = cfg.type;
                        found_root = true;
                        break;
                    }
                }
                break;
            }
        }

                    if (found_root && !applied_morphs.empty()) {
                    String translation = result ? String(result) : current_word;
                    const Morph* morph_to_ptr = &(default_morph_to);
                    for (auto it = applied_morphs.rbegin(); it != applied_morphs.rend(); ++it) {
                        translation = apply_morph(translation, *it, morph_to_ptr, flags);
                        for (const auto& target_var : morph_to_ptr->variations) {
                            if (target_var.morphology == (*it)->morphology) {
                                flags |= target_var.required_flags;
                                break;
                            }
                        }
                    }
                    Word result_word;
                    result_word.word = word;
                    result_word.translation = normalize(translation);
                    result_word.type = found_type;
                    result_word.flags = flags;
                  
                    return result_word;
                }
    }

    // Normal noun lookup
    for (const auto& cfg : configs) {
        if (const char* result = lookup_test(cfg.dict, cfg.length, word.c_str())) {
            String translation = result;
            int word_type = cfg.type;
            uint64_t word_flags = lookupFlags_test(cfg.dict, cfg.length, word.c_str());
            return { word, normalize(translation), word_type, 0, word_flags };
        }
    }

    // Verb fallback
    Word vw = default_verb_lookup(word, sentence, word_index);
    if (vw.type != -1) return vw;

    // TRANSFIX detection
    {
        const Morph* morph_from_ptr = &(default_morph_from);
        for (const auto& var : morph_from_ptr->variations) {
            if (var.type == TRANSFIX) {
                for (const auto& cfg : configs) {
                    const Entry* dict = cfg.dict;
                    size_t dict_length = cfg.length;
                    int dict_type = cfg.type;
                    for (size_t i = 0; i < dict_length; i++) {
                        const Entry& dict_entry = dict[i];
                        String dict_pattern = dict_entry.w;
                        String tmpl = var.ending;
                        String reconstructed = "";
                        size_t d = 0, t = 0;
                        while (d < dict_pattern.size()) {
                            if (dict_pattern[d] == '-') {
                                if (t < tmpl.size() && tmpl[t] == '-') {
                                    d++;
                                    t++;
                                } else if (t < tmpl.size()) {
                                    reconstructed += tmpl[t];
                                    d++;
                                    t++;
                                } else {
                                    d++;
                                }
                            } else {
                                reconstructed += dict_pattern[d];
                                if (t < tmpl.size() && tmpl[t] == '-') {
                                    t++;
                                }
                                d++;
                            }
                        }
                        if (reconstructed == word) {
                            const char* result = lookup_test(dict, dict_length, dict_pattern.c_str());
                            if (result) {
                                uint64_t flags = lookupFlags_test(dict, dict_length, dict_pattern.c_str());
                                String translation = result;
                                const Morph* morph_to_ptr = &(default_morph_to);
                                translation = apply_morph(translation, &var, morph_to_ptr, flags);
                                return { word, normalize(translation), dict_type, 0, flags };
                            }
                        }
                    }
                }
            }
        }
    }

    return Word{ word, normalize(word), -1 };
}


inline String load(const uint8_t* file, size_t size)
{
    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(file);
    const uint8_t* start = ptr;
    const uint8_t* end = ptr + size;
    int current_area = 0;
    const char* s = nullptr;
    const char* r = nullptr;
    String from = "";
    String to = "";
    String temp_affix;
    #ifdef ETL_BUILD
        new (&default_endings) VerbRuleDictionary();
        new (&default_conjugations) VerbConjugationDictionary();
        new (&temp_verb_endings) Vector<AtomString>();
        new (&temp_required_ending) AtomString();
        new (&temp_type) uint8_t(0);
        

        // Clear to initial state
        default_endings.clear();
        default_conjugations.clear();
        temp_verb_endings.clear();
        temp_required_ending.clear();
        temp_type = 0;
        normalizationRuleLength = 0;
    #else
        default_endings.clear();
        default_conjugations.clear();
        temp_verb_endings.clear();
        temp_required_ending.clear();
        temp_affix.clear();
        temp_type = 0;
        normalizationRuleLength = 0;
    #endif

            // Always reset these
            nouns_length = 0;
            verbs_length = 0;
            adjective_length = 0;
            pronoun_length = 0;
            adverb_length = 0;

    uint8_t word_length = 0;
    uint8_t ngram_size = 0x01;
    uint8_t type = 0x00;
    
    static uint64_t target_flags = 0;

    while (ptr < end)
    {
          uint8_t marker = *ptr++;
        
        switch (marker)
        {
            // metadata
            case 0xD0:
            current_area = 0;
            break;
// dicrtionary entries
        case 0xD1:
            current_area = 1;
            break;
// rules, kind of fazed out by now, but we'll see
        case 0xD2:
            current_area = 2;
            break;
// verb endings
        case 0xD3:
            current_area = 3;
            break;
// verb conjugations (target)
        case 0xD4:
            current_area = 4;
            break;
//normalizations
        case 0xD5:
            current_area = 5;
            break;
// morphology (THE MOST IMPORTANT OF THEM ALLLLLL)
     case 0xD6:
    current_area = 6;
    break;
case 0xD7:
    current_area = 7;
    break;

case 0xF0:
{
    if (current_area == 0) // metadata
    {
        word_length = *ptr++;
        s = reinterpret_cast<const char*>(ptr);
        from = String(s);
        ptr += word_length + 1;
    }
        else if (current_area == 1) // dictionary entry
        {
            word_length = *ptr++;
            ngram_size = *ptr++;
            ptr++; // reserved
            s = reinterpret_cast<const char*>(ptr);
            ptr += word_length + 1;
        }
    else if (current_area == 2) // rules (placeholder)
    {
        word_length = *ptr++;
        ptr++;
        ptr += word_length + 1;
    }
    else if (current_area == 3 || current_area == 4) // verb endings / conjugations
    {
        uint8_t ending_count = *ptr++;
        if (current_area == 3) {
            Vector<AtomString> endings;
            for (int i = 0; i < ending_count; i++) {
                s = reinterpret_cast<const char*>(ptr);
                String ending = String(s);
                endings.push_back(ending);
                ptr += strlen(s) + 1;
            }
            temp_verb_endings = endings;
        }
        else { // current_area == 4
            s = reinterpret_cast<const char*>(ptr);
            temp_required_ending = String(s);
            ptr += strlen(s) + 1;
            s = reinterpret_cast<const char*>(ptr);
            temp_affix = String(s);
            ptr += strlen(s) + 1;
        }
    }
    else if (current_area == 5) // normalization rule – ORIGINAL string
    {
        word_length = *ptr++;
        s = reinterpret_cast<const char*>(ptr);
        ptr += word_length + 1;
    }
    else if (current_area == 6 || current_area == 7) {
    // Read rule data
    uint8_t morphology_type = read_byte(ptr);
    
    AtomString affix = read_string(ptr);
    AtomString trigger = read_string(ptr);
    uint8_t affix_type = read_byte(ptr);
    
    // Required flags
    uint8_t req_count = read_byte(ptr);
    uint64_t required_flags = 0;
    for (uint8_t i = 0; i < req_count; i++) {
        required_flags |= read_u64(ptr);
    }

    uint8_t stackable_byte = read_byte(ptr);

    uint64_t result_flags = 0;
    if (current_area == 7) {
        uint8_t low_byte = *ptr++;
        uint8_t high_byte = *ptr++;
        result_flags = low_byte | (high_byte << 8);
    }
    
    // Apply only to list
    uint8_t apply_count = read_byte(ptr);
    Vector<int> apply_only_to;
    for (uint8_t i = 0; i < apply_count; i++) {
        apply_only_to.push_back(read_byte(ptr));
    }
    
    uint8_t result_type = read_byte(ptr);
    uint8_t agreement = read_byte(ptr);
    uint8_t harmony = read_byte(ptr);  

    // Create and push immediately, since both source and target are the same structures
    MorphVariation var;
    var.flag = result_flags;
    var.required_flags = required_flags;
    var.ending = affix;
    var.form = trigger;
    var.type = affix_type;
    var.morphology = morphology_type;
    var.result_type = result_type;
    var.apply_only_to = apply_only_to;
    var.stackable = stackable_byte;
    var.vowel_harmony = nullptr;
    var.agreement = agreement;
 
    if (current_area == 6) {
        default_morph_from.variations.push_back(var);
    } else {
        default_morph_to.variations.push_back(var);
    }
}
    break;
}

case 0xF1:
{
    if (current_area == 0)
    {
        // metadata 
        uint8_t translation_length = *ptr++;
        r = reinterpret_cast<const char*>(ptr);
        ptr += translation_length + 1;
        to = String(r);
    }
    else if (current_area == 1)
    {
        //dictionary translation
        uint8_t translation_length = *ptr++;
        ptr++; // reserved
        r = reinterpret_cast<const char*>(ptr);
        ptr += translation_length + 1;
    }
    else if (current_area == 3 || current_area == 4)
    {
        // what type of affix is it for verb conjgation
        temp_type = *ptr++;
    }  else if (current_area == 5)
    {
        // Normalization rule - replacement string
        uint8_t replacement_length = *ptr++;
        r = reinterpret_cast<const char*>(ptr);
        ptr += replacement_length + 1;
    }
    break;
}

case 0xF2:
{
    type = *ptr++;

    if (current_area == 0) {
        default_multibyte = (type == 0x00);
    }
    else if (current_area == 1) {
        // dictionary type handling
    }
    else if (current_area == 2) {
        // default_normalizationRules[normalizationRuleLength++] = 
        //     NormalizationRule{s, r, type};
    }
    else if (current_area == 3) {
            if (default_endings.size() >= MAX_VERB_ENDINGS) {
        
                break;  // dont add if overflowing
            }
        // Create VerbRule from temp data
        VerbRule rule;
        rule.endings = temp_verb_endings;
        rule.type = temp_type;
        rule.form = type; // form flags
        
        default_endings.push_back(rule);
        // Clear 
        temp_verb_endings.clear();
    }
    else if (current_area == 4) {
        if (default_conjugations.size() >= MAX_VERB_CONJUGATIONS) {
        break; 
    }
    
        VerbConjugation conj;
        conj.form = type; // form flags (INFINITIVE, PAST, etc.)
        conj.type = temp_type; // SUFFIX/PREFIX
        conj.required_ending = temp_required_ending;
        conj.affix = temp_affix;
        conj.vowel_harmony = nullptr;
    
         default_conjugations.push_back(conj);
    }
    else if (current_area == 5) {
    if (s && r && normalizationRuleLength < MAX_NORMALIZATION_RULES) {
        new (&default_normalizationRules[normalizationRuleLength]) NormalizationRule();
        new (&default_normalizationRules[normalizationRuleLength].from) String(s);
        new (&default_normalizationRules[normalizationRuleLength].to) String(r);
        default_normalizationRules[normalizationRuleLength].type = type;
        normalizationRuleLength++;
    }
}
    break;
}
           case 0xF3:
{
    
    if (current_area == 0)
    {
        uint8_t translation_length = *ptr++;
        r = reinterpret_cast<const char*>(ptr);
        ptr += translation_length + 1;
        to = String(r);
    }
    else if (current_area == 1)
    {
        // Read flag count (2 bytes)
        uint16_t flag_count = 0;
        flag_count |= *ptr++;
        flag_count |= (*ptr++) << 8;
        
        target_flags = 0;
        
        // Read each flag (2 bytes each)
        for (uint16_t i = 0; i < flag_count; i++) {
            uint16_t flag_value = 0;
            flag_value |= *ptr++;
            flag_value |= (*ptr++) << 8;
            target_flags |= flag_value;
        }
        
        // Now process the dictionary entry
        switch (ngram_size)
        {
               case 0x01:
                    if(type == 0x03) {  // VERB goes to separate verbs array
                        Verb e = {s, r, 0, target_flags};
                        new (&default_verbs[verbs_length]) Verb();
                        default_verbs[verbs_length++] = e;
                    } else {  // Everything else (NOUN, ADJECTIVE, PRONOUN, ADVERB) goes to dictionary
                        Entry e = {s, r, 0, target_flags, nullptr, nullptr, nullptr, nullptr};
                        e.word_type = type;
                        new (&default_nouns[nouns_length]) Entry();
                        default_nouns[nouns_length++] = e;
                    }
                break;
                
            case 0x02:
            case 0x03:
                {
                    Entry e = {s, r, 0, target_flags};
                    default_fixed_ngrams[ngrams_length++] = e;
                }
                break;
        }
        
        target_flags = 0;
    }
    else if (current_area == 2)
    {
        // handle area 2
    }
    else if (current_area == 3)
    {
        uint8_t target_length = *ptr++;
        r = reinterpret_cast<const char*>(ptr);
        ptr += target_length + 1;
    }
    break; 
}


        }
    }

    
// std::cout << "Nouns: " << nouns_length << "\n";
// std::cout << "Adjectives: " << adjective_length << "\n";
// std::cout << "Pronouns: " << pronoun_length << "\n";
// std::cout << "Multi-word entries: " << ngrams_length << "\n";
// std::cout << "Verbs: " << verbs_length << "\n";
// std::cout << "Verb endings: " << default_endings.size() << "\n";
// std::cout << "Verb conjugations: " << default_conjugations.size() << "\n";
// std::cout << "Normalization rules: " << normalizationRuleLength << "\n";
// std::cout << "Multibyte? " << (default_multibyte == 0 ? "No" : "Yes") << "\n";
// After loading, before returning

    // std::cout << "Source morph variations: " << default_morph_from.variations.size() << std::endl;
    // std::cout << "Target morph variations: " << default_morph_to.variations.size() << std::endl;

    unsigned int counts[4] = {0, 0, 0, 0}; // NOUN, ADJ, PRONOUN, ADVERB
for (unsigned int i = 0; i < nouns_length; ++i) {
    if (default_nouns[i].word_type == 0x00) counts[0]++;
    else if (default_nouns[i].word_type == 0x01) counts[1]++;
    else if (default_nouns[i].word_type == 0x04) counts[2]++;
    else if (default_nouns[i].word_type == 0x08) counts[3]++;
}
printf("Loaded translator: %s > %s\n", from.c_str(), to.c_str());
printf("  Dictionary: %d nouns, %d adjectives, %d pronouns, %d adverbs, %d verbs\n", 
       counts[0], counts[1], counts[2], counts[3], verbs_length);
printf("  Verb endings: %zu groups, %zu conjugations\n", 
       default_endings.size(), default_conjugations.size());
printf("  Morphology: %zu source, %zu target\n", 
       default_morph_from.variations.size(), default_morph_to.variations.size());
printf("  Normalization rules: %d\n", normalizationRuleLength);
printf("  Multibyte: %s\n", (default_multibyte == 0 ? "No" : "Yes"));


return String("Loaded translator: " + from + " > " + to + "\n");
}
inline String translate(const char* sentence,
                                      int script = 0,
                                      bool auto_correct = false)
{
    if (sentence == nullptr) {
        return "";
    }

    if (sentence[0] == '\0') {
        return "";
    }

    char buffer[250];

    strncpy(buffer, sentence, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    if (buffer[0] == '\0') {
        return "";
    }

  to_lower(buffer);
  Vector<String> arr; 
 if(default_multibyte == true) { 
    arr = tokenize_cjk(String(buffer));
    Vector<String> verb_combined; 
    if (!(verbs_length < 1) && !default_endings.empty()) {
        DEFAULT_COMBINE_VERB_TOKENS(arr, verb_combined, default_verbs, default_endings, 3, 2); 
        Vector<String> final_combined; 
        size_t i = 0; 
        while (i < verb_combined.size()) { 
            bool found = false; 
            for (size_t len = 4; len >= 1; --len) { 
                if (i + len > verb_combined.size()) continue; 
                String cand; 
                for (size_t k = 0; k < len; ++k) cand += verb_combined[i + k];
               if (lookup_test(default_nouns, nouns_length, cand.c_str())
                ) {
                    final_combined.push_back(cand); 
                    i += len; 
                    found = true; 
                    break; 
                } 
            } 
            if (!found) { 
                final_combined.push_back(verb_combined[i]); 
                ++i; 
            } 
        } 
        arr = final_combined;
    } else {
        verb_combined = arr; 
        arr = verb_combined;
    }
} else { 
    arr = tokenize(String(buffer)); 
} 

    if (arr.empty()) {
        return "";
    }

    String translated =
        trigramLookup(default_fixed_ngrams,
                      arr,
                      default_reorderHelpers,
                      default_nounLookup);

    return translated;
}

extern Homonym homonyms[];
extern const size_t homonymCount;
// this can be global, since specific cases are defined on the structs and not on the function itself
inline String semantics(const Vector<String>& sentence,
                             const Vector<int>& word_types,
                             size_t index,
                             Homonym* homonyms,
                             size_t numHomonyms)
{
    if (index >= sentence.size()) return "";

    const String& w = sentence[index];
    for (size_t h = 0; h < numHomonyms; ++h) {
        Homonym& hom = homonyms[h];
        if (!hom.word) continue;
        if (String(hom.word) != w) continue;

        // Reset outcome scores
        for (size_t j = 0; j < hom.num_outcomes; ++j) {
            if (hom.outcomes && j < hom.num_outcomes) {
                hom.outcomes[j].score = 0.0f;
            }
        }

        // Check context words
        int offsets[] = {-2, -1, 1, 2};
        for (int o : offsets) {
            int nearbyIdx = static_cast<int>(index) + o;
            if (nearbyIdx < 0 || nearbyIdx >= static_cast<int>(sentence.size())) continue;

            const String& nearby = sentence[nearbyIdx];
            if (nearby.empty()) continue;

            size_t currentOutcome = 0;
            if (!hom.tokens) continue;

            for (size_t tidx = 0; tidx < hom.num_tokens && currentOutcome < hom.num_outcomes; ++tidx) {
                if (!hom.tokens[tidx]) continue;
                String token = hom.tokens[tidx];
     
                if (token == "$") { 
                    ++currentOutcome; 
                    continue; 
                }

                if (nearby == token) {
                    bool forbidden = false;

                    int prevIdx = nearbyIdx - 1; 
                    int prevType = (prevIdx >= 0 && prevIdx < (int)word_types.size()) ? word_types[prevIdx] : -1;

                    if (prevType >= 0 && hom.forbidden_previous_type && currentOutcome < hom.num_outcomes) {
                        if (prevType == hom.forbidden_previous_type[currentOutcome]) {
                            forbidden = true;
                        }
                    }

                    if (!forbidden && hom.outcomes && currentOutcome < hom.num_outcomes) {
                        hom.outcomes[currentOutcome].score += 1.0f;
                    }

                    break;
                }

            }
        }

        // Pick best outcome
        float bestScore = -1.0f;
        String bestWord = w;
        for (size_t j = 0; j < hom.num_outcomes; ++j) {
            if (hom.outcomes && j < hom.num_outcomes) {
                Outcome& o = hom.outcomes[j];
                if (o.score > bestScore) {
                    bestScore = o.score;
                    bestWord = o.word ? String(o.word) : w;
                }
            }
        }
        return bestWord;
    }

    return w;
}

#endif
