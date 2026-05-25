#ifndef LANG_IT_H
#define LANG_IT_H


#include <string>
#include <cstdint>
#include <vector>
#include <cstring>
#include <cstddef>
// if you use ETL, set the fixed size of vectors, strings and structure arrays
#define MAX_STRING_LENGTH 150
#define MAX_VECTOR_LENGTH 14
#define MAX_VERB_ENDINGS 12
#define MAX_VERB_CONJUGATIONS 5
#define MAX_ENTRIES 30
#define MAX_NORMALIZATION_RULES 10


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
            uint16_t flags;
            HarmonyTable* vowel_harmony;
        } VerbConjugation;

        


    using VerbRuleDictionary = etl::vector<VerbRule, MAX_VERB_ENDINGS>;
    using VerbConjugationDictionary = etl::vector<VerbConjugation, MAX_VERB_CONJUGATIONS>;
    

    struct MorphVariation {
    uint16_t  flag;
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
            uint16_t flags;
            HarmonyTable* vowel_harmony;
        } VerbConjugation;


    using VerbRuleDictionary = std::vector<VerbRule>;
    using VerbConjugationDictionary = std::vector<VerbConjugation>;
    
struct MorphVariation {
    uint16_t  flag;
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

    inline int string_to_int(const String& str) {
        return std::stoi(str);
    }
    
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
    NONE = 14
};

// wait i'll add this as a note so i don't get lost
// the morph objects can take STACKABLE or NOT_STACKABLE
// if they can stack, not only you look for root + morph, you look for all the morphologies that are also stackable
// like morph + root + morph (e.g malay: ber-$-an), or stuff like root + morph + morph (e.g turkish: $-lar-ım)
// if found, you simply pass the root without all of them to the morph too
// does that make sense? maybe, its 4:05 am i haven't slept
// IT WORKSSSSSSSSSSSSSSSSS, tested with portuguese ('inh' -> diminutive_neutral, 'o' -> masc_gender, 's' -> plural)
//successfully turns cachorr-inh-o-s into little dogs ( i lwk dont even remember why that works? but the apply morph hasnt done anythign crazy yetm, so i dont care)
#define LOOKUP(DICTIONARY, TYPE, WORD, MORPH_FROM, MORPH_TO, SCRIPT)          \
{                                                                       \
    /* Direct lookup first, try any of the scripts */                                          \
    if (const char* result = lookup(DICTIONARY, WORD.c_str(), 0)) {       \
        uint16_t flags = lookupFlags(DICTIONARY, WORD.c_str());        \
        return { WORD, normalize(result), TYPE, 0, flags };            \
    }                                                                   \
    else if (const char* result = lookup(DICTIONARY, WORD.c_str(), 1)) {       \
        uint16_t flags = lookupFlags(DICTIONARY, WORD.c_str());        \
        return { WORD, normalize(result), TYPE, 0, flags };            \
    }                                                                   \
                                                                        \
    /* NON-CONCATENATIVE TEMPLATIC DETECTION */                         \
    {                                                                   \
        const Morph* morph_from_ptr = &(MORPH_FROM);                   \
        for (const auto& var : morph_from_ptr->variations) {           \
            if (var.type == TRANSFIX) {              \
                for (const auto& dict_entry : DICTIONARY) {            \
                    String dict_pattern = dict_entry.w;           \
                    String tmpl = var.ending;                     \
                    WordString reconstructed = "";                    \
                    size_t d = 0, t = 0;                               \
                    while (d < dict_pattern.size()) {                  \
                        if (dict_pattern[d] == '_') {                  \
                            if (t < tmpl.size() && tmpl[t] == '_') {   \
                                /* both underscore: skip */            \
                                d++; t++;                              \
                            } else if (t < tmpl.size()) {              \
                                /* template has letter: add it */      \
                                reconstructed += tmpl[t];              \
                                d++; t++;                              \
                            } else {                                   \
                                /* no template left: skip underscore */ \
                                d++;                                   \
                            }                                          \
                        } else {                                       \
                            /* consonant: add it */                    \
                            reconstructed += dict_pattern[d];          \
                            /* if template has underscore, consume it */ \
                            if (t < tmpl.size() && tmpl[t] == '_') {   \
                                t++;                                   \
                            }                                          \
                            d++;                                       \
                        }                                              \
                    }                                                  \
                    /* ignore any remaining template letters */        \
                    if (reconstructed == WORD) {                       \
                        const char* result = lookup(DICTIONARY, dict_pattern.c_str()); \
                        if (result) {                                 \
                            uint16_t flags = lookupFlags(DICTIONARY, dict_pattern.c_str()); \
                            String translation = result;         \
                            const Morph* morph_to_ptr = &(MORPH_TO);   \
                            translation = apply_morph(translation, &var, morph_to_ptr, flags); \
                            return { WORD, normalize(translation), TYPE, 0, flags }; \
                        }                                             \
                    }                                                 \
                }                                                     \
            }                                                         \
        }                                                             \
    }                                                                 \
                                                                        \
    /* REGULAR MORPHOLOGICAL DETECTION */                              \
    {                                                                   \
        const Morph* morph_from_ptr = &(MORPH_FROM);                   \
        String current_word = WORD;                               \
        Vector<const MorphVariation*> applied_morphs;             \
        bool found_root = false;                                       \
        const char* result = nullptr;                                  \
        uint16_t flags = 0;                                            \
        /* i'll limit it for 10 right now, but when agglutinative languages i'll inflate */ \
        int safety = 10;                                               \
        while (!found_root && safety-- > 0) {                          \
            MorphResult p = detect_morph(current_word, morph_from_ptr); \
            if (p.matched_variation != nullptr) {                      \
                if (p.matched_variation->stackable == STACKABLE) {     \
                    /* check if it is stackable*/                      \
                    applied_morphs.push_back(p.matched_variation);     \
                    current_word = p.root;                             \
                    /* Try lookup with stripped word */                \
                    if ((result = lookup(DICTIONARY, current_word.c_str()))) { \
                        flags = lookupFlags(DICTIONARY, current_word.c_str()); \
                        found_root = true;                             \
                        break;                                         \
                    }                                                  \
                } else {                                               \
                    /* Non-stackable, just get it and stop */          \
                    applied_morphs.push_back(p.matched_variation);     \
                    if ((result = lookup(DICTIONARY, p.root.c_str()))) { \
                        flags = lookupFlags(DICTIONARY, p.root.c_str()); \
                        found_root = true;                             \
                    }                                                  \
                    break;                                             \
                }                                                      \
            } else {                                                   \
                break;                                                 \
            }                                                          \
        }                                                              \
        if (found_root) {                                              \
            String translation = result;                          \
            const Morph* morph_to_ptr = &(MORPH_TO);                   \
            for (auto it = applied_morphs.rbegin(); it != applied_morphs.rend(); ++it) { \
                translation = apply_morph(translation, *it, morph_to_ptr, flags); \
            }                                                          \
            return { WORD, normalize(translation), TYPE, 0, flags };   \
        }                                                              \
    }                                                                  \
}
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
   uint16_t flags;
   String addition; 

} GenitiveConstruction;

typedef struct  {
   int type; // how does it act? e.g suffix, prefix, prev word
   String addition; // like "[o] cachorro, [the] dog, hund[en]"
   uint16_t flags; // e.g in swedish EN vs ETT (utrum vs neutrum)
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
    if (v.translation && *v.translation) {                             \
        uint16_t flags = v.flags;                                      \
        return { WORD, normalize(v.translation), VERB, 0, flags };     \
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
                        if (v.translation && *v.translation) {         \
                            String translation = v.translation;   \
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
            if (v.translation && *v.translation) {                    \
                String translation = v.translation;              \
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
        uint16_t flags = 0;                                           \
        int safety = 10;                                              \
        while (!found_root && safety-- > 0) {                         \
            MorphResult p = detect_morph(current_word, morph_from_ptr); \
            if (p.matched_variation != nullptr) {                     \
                if (p.matched_variation->stackable == STACKABLE) {    \
                    applied_morphs.push_back(p.matched_variation);    \
                    current_word = p.root;                            \
                    Verb v = verb_lookup(DICTIONARY, current_word.c_str()); \
                    if (v.translation && *v.translation) {            \
                        result = v.translation;                       \
                        flags = v.flags;                              \
                        found_root = true;                            \
                        break;                                        \
                    }                                                 \
                } else {                                              \
                    applied_morphs.push_back(p.matched_variation);    \
                    Verb v = verb_lookup(DICTIONARY, p.root.c_str()); \
                    if (v.translation && *v.translation) {            \
                        result = v.translation;                       \
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


/* ------- GLOBAL CORE FUNCTIONS -----------
|           all pairs use                  |  
------------------------------------------*/


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
    uint16_t orig_flags; // flags for original word, just remembered that in languages that have same linguistic features but with variation (e.g gender in portuguese and russian) we need to know the flags for both the original word and the translation to make decisions.
    uint16_t flags;
    const char* w2; // not elegant at all, but handles multi script, so we can translate between three different scripts to other three different, like [(bopomofo, hanzi, pinyin) -> (kana, kanji, romaji)]
    const char* w3;
    const char* t2;
    const char* t3;
} Entry;


struct Verb {
    const char* root;       
    const char* translation; 
    int type;      
    uint16_t flags;
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
   FOCUS_MORPH = 12

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
        const String& suffix = var.form;

        if (word.size() >= suffix.size() &&
            word.compare(word.size() - suffix.size(), suffix.size(), suffix) == 0)
        {
            String root = word.substr(0, word.size() - suffix.size());
            return { root, &var };
        }
    }
    return { word, nullptr };
}


inline String apply_case(
    const String& translation,
    const CaseVariation* from_var,
    const Case* case_to,
    uint8_t word_gender) 
{
    if (!case_to || case_to->variations.empty())
        return translation; 


 if (!from_var && case_to) {
     
    // NON-CASE → CASE
   for (const auto& var : case_to->variations) {
    if (!from_var || var.flag == from_var->flag) {      
        if (word_gender == 0 || (var.gender & word_gender)) {
            const String& ending = var.ending;
            const String& form   = var.form;
            String affix = "";

                if(var.v_h != nullptr){
                     const HarmonyTable& v_h = *(var.v_h); 
                    affix = checkVowelHarmony(translation, form, v_h);
                    }
            if (translation.size() >= ending.size() &&
                translation.compare(translation.size() - ending.size(), ending.size(), ending) == 0) 
            {
                   String result = translation.substr(0, translation.size() - ending.size());
    
                    result += affix;
        
                    return result;
            }
        }
    }
}

    for (const auto& var : case_to->variations) {
        if (word_gender == 0 || (var.gender & word_gender)) {
            const String& ending = var.ending;
            const String& form   = var.form;

            if (ending.length() <= translation.length()){
                String result;
                result = translation.substr(0, translation.size() - ending.size());
                result += form;
                return result;
                ;}
            else
                return form;
        }
    }
    }else if (from_var && !case_to) {
    // CASE → NON-CASE
    }else if (from_var && case_to) {
    // CASE → CASE
}



    // fallback: just use the first variation
    const auto& var = case_to->variations[0];
    if (var.ending.length() <= translation.length())
        return translation.substr(0, translation.length() - var.ending.length()) + var.form;
    else
        return var.form;
}



inline MorphResult detect_morph(const String& word, const Morph* morph_from) {
    if (!morph_from) {
        return { word, nullptr };
    }

    for (const auto& var : morph_from->variations) {
        
        int type = var.type;  

        switch(type){
       
         case PREV_WORD:
{}
        break;
        case NEXT_WORD:

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
    uint8_t word_gender)
{
    if (!morph_to || !from_var)
        return translation;

    int source_morph = from_var->morphology; 
         String result;
    for (const auto& var : morph_to->variations) {
        
        if (var.morphology == source_morph) { 
        
        const String& ending = var.ending;
        const String& form   = var.form;
        String affix = ending;
                if(var.vowel_harmony != nullptr){
                       const HarmonyTable& v_h = *(var.vowel_harmony);
                        affix = checkVowelHarmony(translation, ending, v_h);
                    // ifd theres valid vowel harmony, replace the vowel
                }
                
                // infixes and reduplication use a specific format for trigger so i'll do it apart
                if(var.type == TOTAL_REDUPLICATION){
                        const String& trigger = var.form;
                        int times =string_to_int(trigger);
                        String result = translation;
                        switch (times)
                        {
                        case 2:       
                           result = translation + translation;
                        break;
                        case 3:       
                           result = translation + translation + translation;
                        break;
                        default:
                            break;
                        }
                        
                           return result;
                }
                if(var.type == PARTIAL_REDUPLICATION_PREFIX) {
                    String pattern = var.ending;  // like "CVC", "CV", "VC", etc.
                    String trigger = var.form;    // "B" for beginning, "E" for ending
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
                                }
                                else if (patternChar == 'V' && !isVowel(wordChar)) {
                                    matches = false;
                                    break;
                                }
                            }
                            if (matches) {
                                matchedPart = translation.substr(0, pattern.length());
                            }
                        }
                    }
                    else if (trigger == "E") {
                        // Look at ENDING of translation to find pattern
                        if (translation.length() >= pattern.length()) {
                            bool matches = true;
                            int startPos = translation.length() - pattern.length();
                            for (int i = 0; i < pattern.length(); i++) {
                                char patternChar = pattern[i];
                                char wordChar = translation[startPos + i];
                                
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
                                matchedPart = translation.substr(startPos);
                            }
                        }
                    }
                    
                    // add the reduplicated pattern to  BEGINNING ( cause PREFIX)
                    if (!matchedPart.empty()) {
                        result = matchedPart;
                        result += translation;
                    }
                    
                    return result;
                }
               if(var.type == PARTIAL_REDUPLICATION_SUFFIX) {
                String pattern = var.ending;  // like "CVC", "CV", "VC", etc.
                String trigger = var.form;    // "B" for beginning, "E" for ending
                String result = translation;
                String matchedPart;
                
                // find the pattern either on B eginning or E nding
                if (trigger == "B") {
                    // Look at BEGINNING of translation to find pattern
                    if (translation.length() >= pattern.length()) {
                        bool matches = true;
                        for (int i = 0; i < pattern.length(); i++) {
                            char patternChar = pattern[i];
                            char wordChar = translation[i];
                            
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
                            matchedPart = translation.substr(0, pattern.length());
                        }
                    }
                }
                else if (trigger == "E") {
                    // look at ending
                    if (translation.length() >= pattern.length()) {
                        bool matches = true;
                        int startPos = translation.length() - pattern.length();
                        for (int i = 0; i < pattern.length(); i++) {
                            char patternChar = pattern[i];
                            char wordChar = translation[startPos + i];
                            
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
                            matchedPart = translation.substr(startPos);
                        }
                    }
                }
                
               // add the reduplicated pattern to  ENDING ( cause SUFFIX)
                if (!matchedPart.empty()) {
                    result = translation;
                    result += matchedPart;
                }
                
                return result;
            }
        if (var.type == INFIX) {
                if (var.flag == from_var->flag) {
                    const String& infix_to_add = var.ending;
                    const String& trigger = var.form;
                    String result = translation;
                    
                    // Helper to convert character position to byte position
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
                    }
                    else if (trigger == "BFV") {
                        Letter first_vowel = getFirstVowel(result);
                        if (first_vowel.byte_pos >= 0) {
                            result.insert(first_vowel.byte_pos, infix_to_add);
                        }
                    }
                    else if (trigger == "BLV") {
                        Letter last_vowel = getLastVowel(result);
                        if (last_vowel.byte_pos >= 0) {
                            result.insert(last_vowel.byte_pos, infix_to_add);
                        }
                    }
                    
                    return result;
                }
            }
                else if ((form.empty() || 
                    (translation.size() >= form.size() &&
                    translation.compare(translation.size() - form.size(), form.size(), form) == 0)) && 
                    var.flag == from_var->flag){
                    switch (var.type) {
                        
                        case SUFFIX:
                            result = translation.substr(0, translation.size() - form.size());
                            result += affix;
                            return result;
                            
                        case PREFIX:
                        
                            result = affix;
                            result += translation.substr(form.size());
                            
                            return result;
                            
                            break;
                            
                        case PREV_WORD:
                            result += form;
                            result += " ";
                            return translation;
                        break;
                            
                        case NEXT_WORD:
                            result += translation;
                            result += affix;
                            return result;
                        break;

                        case TRANSFIX:
{
    String dict_pattern = translation;
    String template_pattern = var.ending;
    String result = "";
    size_t dict_pos = 0;
    size_t temp_pos = 0;
    
    /* Apply template to translation */
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
    
    return result;
}
break;

                        case CIRCUMFIX:
                     {   const String& full_circumfix = affix; 
                        String delimiter = "$"; //common delimiter i use is '$'
                        //get the prefix
                        String pre = full_circumfix.substr(0, full_circumfix.find(delimiter));
                            //get the suffix
                        String suf = full_circumfix.substr((full_circumfix.find(delimiter) + 1), full_circumfix.length());
                           
                          return pre + translation + suf; } 
                         break;
                            
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
#define CASE_DEF(name,...) inline Case name = {__VA_ARGS__}


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
                if (v.translation && *v.translation) { \
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
                if (v.translation && *v.translation) { \
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
            if (v.translation && *v.translation) {                         \
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

#define RULE(str, ...) \
    if (rule(str, sentence_arr, reordered_arr, i, ##__VA_ARGS__)) continue;


#define DEFAULT()\
             reordered_arr.push_back(Word{ sentence_arr.at(i).word, normalize(sentence_arr.at(i).translation), sentence_arr.at(i).type,    sentence_arr.at(i).orig_flags, sentence_arr.at(i).flags});\

            #define HANDLE_CASE(INFO_ARG, FROM_CASE, TO_CASE) \
    if (!reordered_arr.empty()) { \
        for (size_t i = 0; i + 1 < reordered_arr.size(); ++i) { \
            auto &current = reordered_arr.at(i); \
            auto &next = reordered_arr.at(i + 1); \
            Word current_translation_temp = reordered_arr.at(i); \
            Word next_translation_temp = reordered_arr.at(i + 1); \
            if (current.type == 3 && (next.type == 0 || next.type == 4)) { \
                uint8_t f = lookupFlags(nouns, next.word.c_str()); \
                CaseResult g = detect_case(current.word, FROM_CASE); \
                if ((INFO_ARG)->clause_order_to == SVO) { \
                    next.translation = apply_case(next.translation, g.matched_variation, TO_CASE, f); \
                } else if ((INFO_ARG)->clause_order_to == SOV) { \
                    current = Word{next.word,apply_case(next.translation, g.matched_variation, TO_CASE, f), next.type, next.orig_flags, next.flags}; \
                    next = current_translation_temp; \
                } \
            } \
        } \
    }




// have to rename this to general abstractions, sinc it doesnt handle only posession, but also adjective order, definiteness and futurally more
#define HANDLE_POSSESSION(INFO_ARG, ARR, MORPH_TO_OBJ) \
    do { \
       if ((ARR).size() >= 1) { \
            bool found = false; \
            for (int p_i = 0; p_i < (INFO_ARG)->def_from_count && !found; ++p_i) { \
                Definiteness def_from = (INFO_ARG)->def_from[p_i]; \
                int type_from = def_from.type; \
                if(type_from == PREFIX || type_from == SUFFIX || type_from == CIRCUMFIX || type_from == INFIX) { \
                    for (size_t i = 0; i < (ARR).size() && !found; ++i) { \
                        if (type_from == SUFFIX) { \
                            if(endsWith((ARR)[i].word, def_from.addition)) { \
                                 found = true; \
                            } \
                        } \
                        if (type_from == PREFIX) { \
                            if((ARR)[i].word.substr(0, def_from.addition.length()) == def_from.addition) { \
                                found = true; \
                            } \
                        } \
                    } \
             } else { \
    for (size_t i = 1; i < (ARR).size() && !found; ++i) { \
        if (type_from == PREV_WORD) { \
                    if ((ARR)[i - 1].word == def_from.addition) { \
                        for (int t_i = 0; t_i < (INFO_ARG)->def_to_count && !found; ++t_i) { \
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
                                 }(ARR).erase((ARR).begin() + i); \
                                    found = true; \
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
   /* Adjective-Noun Agreement Enforcement */ \
if ((ARR).size() >= 2) { \
    /* Quick check: is there any agreement morphology defined? */ \
    bool has_agreement_morph = false; \
    for (const auto& var : MORPH_TO_OBJ.variations) { \
        if (var.agreement == AGREEMENT) { \
            has_agreement_morph = true; \
            break; \
        } \
    } \
    if (has_agreement_morph) { \
        /* create a mask for each word to track what morphologies have been applied */ \
        Vector<int> word_applied_mask((ARR).size(), 0); \
        /* first pass: find the noun and get its flags AND applied morphologies */ \
        for (size_t i = 0; i < (ARR).size(); ++i) { \
            /* if we find a noun */ \
            if ((ARR)[i].type == NOUN || (ARR)[i].type == PRONOUN) { \
                /* skip if translation is empty or same as word (untranslated) */ \
                if ((ARR)[i].translation.empty() || (ARR)[i].translation == (ARR)[i].word) { \
                    continue; \
                } \
                /* get all flags from this noun */ \
                uint16_t noun_flags = (ARR)[i].flags; \
                /* collect morphologies to apply from BOTH sources */ \
                Vector<const MorphVariation*> morphs_to_apply; \
                /* source 1: from noun's flags (gender, etc.) */ \
                for (const auto& var : MORPH_TO_OBJ.variations) { \
                    if (var.agreement == AGREEMENT && (noun_flags & var.flag)) { \
                        morphs_to_apply.push_back(&var); \
                    } \
                } \
                /* source 2: from noun's translation suffixes (plural, etc.) */ \
                for (const auto& var : MORPH_TO_OBJ.variations) { \
                    if (var.agreement == AGREEMENT) { \
                        if (var.type == SUFFIX) { \
                            if ((ARR)[i].translation.size() >= var.ending.size() && \
                                (ARR)[i].translation.compare((ARR)[i].translation.size() - var.ending.size(), var.ending.size(), var.ending) == 0) { \
                                /* avoid duplicates */ \
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
                /* ONLY sort if we have at least 2 morphologies to apply */ \
                if (morphs_to_apply.size() >= 2) { \
                    /* sort so that morphologies with non-empty form (replacements) come before those with empty form (additions) */ \
                    do { \
                        size_t n = morphs_to_apply.size(); \
                        for (size_t ii = 0; ii < n - 1; ii++) { \
                            for (size_t jj = 0; jj < n - ii - 1; jj++) { \
                                const MorphVariation* a = morphs_to_apply[jj]; \
                                const MorphVariation* b = morphs_to_apply[jj + 1]; \
                                bool swap_needed = false; \
                                /* swap if a has empty form and b has non-empty form (move non-empty forward) */ \
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
                /* only proceed if we have morphologies to apply */ \
                if (!morphs_to_apply.empty()) { \
                    /* look forwards at ALL words after this noun */ \
                    for (size_t j = i + 1; j < (ARR).size(); ++j) { \
                        /* if we find an adjective or article before this noun */ \
                        if ((ARR)[j].type == ADJECTIVE || (ARR)[j].type == ARTICLE || (ARR)[j].type == PRONOUN) { \
                            String translation = (ARR)[j].translation; \
                            for (const auto& morph : morphs_to_apply) { \
                                /* only apply if this morphology type hasn't been applied to this word yet */ \
                                if (!(word_applied_mask[j] & (1 << morph->morphology))) { \
                                    translation = apply_morph(translation, morph, &MORPH_TO_OBJ, 0); \
                                    word_applied_mask[j] |= (1 << morph->morphology); \
                                } \
                            } \
                            (ARR)[j].translation = translation; \
                        } \
                    } \
                    /* look backwards at ALL words before this noun */ \
                    for (int j = i - 1; j >= 0; --j) { \
                        /* if we find an adjective or article before this noun */ \
                        if ((ARR)[j].type == ADJECTIVE || (ARR)[j].type == ARTICLE || (ARR)[j].type == PRONOUN) { \
                            String translation = (ARR)[j].translation; \
                            for (const auto& morph : morphs_to_apply) { \
                                /* only apply if this morphology type hasn't been applied to this word yet */ \
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
        if ((ARR).size() >= 3) { \
            for (int p_i = 0; p_i < (INFO_ARG)->gc_from_count; ++p_i) { \
                GenitiveConstruction gc_from = (INFO_ARG)->gc_from[p_i]; \
                if (gc_from.type != MIDDLE_WORD) continue; \
                \
                bool found = false; \
                for (size_t i = 0; i + 2 < (ARR).size() && !found; ++i) { \
                    if ((ARR)[i + 1].word != gc_from.addition) continue; \
                    size_t idxA, idxB; \
                    if (gc_from.order == POSSESSED_FIRST) { \
                        idxA = i;      /* A is first (possessed) */ \
                        idxB = i + 2;  /* B is last (owner) */ \
                    } else { /* OWNER_FIRST */ \
                        idxA = i + 2;  /* A is last (possessed) */ \
                        idxB = i;      /* B is first (owner) */ \
                    } \
                    Word A = (ARR)[idxA];  /* possessed */ \
                    Word B = (ARR)[idxB];  /* owner */ \
                    \
                    /* Apply target construction */ \
                    if ((INFO_ARG)->gc_to_count == 0) break; \
                    \
                    /* Find the matching target construction based on flags */ \
                    const GenitiveConstruction* matching_gc_to = nullptr; \
                    for (int t_i = 0; t_i < (INFO_ARG)->gc_to_count; ++t_i) { \
                        GenitiveConstruction gc_to = (INFO_ARG)->gc_to[t_i]; \
                        /* If gc_from has a flag, match it; otherwise match the default (0) */ \
                        if (gc_from.flags == 0) { \
                            if (gc_to.flags == 0) { \
                                matching_gc_to = &(INFO_ARG)->gc_to[t_i]; \
                                break; \
                            } \
                        } else { \
                            if (gc_to.flags & gc_from.flags) { \
                                matching_gc_to = &(INFO_ARG)->gc_to[t_i]; \
                                break; \
                            } \
                        } \
                    } \
                    \
                    /* If no matching target found, use the first one as fallback */ \
                    if (!matching_gc_to) { \
                        matching_gc_to = &(INFO_ARG)->gc_to[0]; \
                    } \
                    \
                    GenitiveConstruction gc_to = *matching_gc_to; \
                    \
                    /* Add the flag from gc_from to the possessed word */ \
                    Word modified_possessed = A; \
                    modified_possessed.flags |= gc_from.flags; \
                    \
                    /* Build replacement words according to target order */ \
                    Vector<Word> replacement; \
                    if (gc_to.order == POSSESSED_FIRST) { \
                        if (gc_to.type == SUFFIX) { \
                            /* Attach suffix directly to the possessed word */ \
                            Word combined = modified_possessed; \
                            combined.translation = modified_possessed.translation + gc_to.addition; \
                            replacement.push_back(combined); \
                            replacement.push_back(B); \
                        } else { \
                            replacement.push_back(modified_possessed); \
                            replacement.push_back(Word{gc_to.addition, gc_to.addition, 5, 0, 0}); \
                            replacement.push_back(B); \
                        } \
                    } else { /* OWNER_FIRST */ \
                        if (gc_to.type == SUFFIX) { \
                            /* Attach suffix directly to the owner word */ \
                            Word combined = B; \
                            combined.translation = B.translation + gc_to.addition; \
                            replacement.push_back(combined); \
                            replacement.push_back(modified_possessed); \
                        } else { \
                            replacement.push_back(B); \
                            replacement.push_back(Word{gc_to.addition, gc_to.addition, 5, 0, 0}); \
                            replacement.push_back(modified_possessed); \
                        } \
                    } \
                    \
                    /* Replace the three old words with the new ones */ \
                    (ARR).erase((ARR).begin() + i, (ARR).begin() + i + 3); \
                    (ARR).insert((ARR).begin() + i, replacement.begin(), replacement.end()); \
                    \
                    found = true; \
                    break;\
                } \
                if (found) break;  \
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
    OBLIQUE_PRONOUN = 11,
    PREPOSITION = 8,
    ARTICLE = 9,
    ADVERB = 13,
    POSSESSIVE_PRONOUN = 40,
    UNKNOWN = 99
};

enum VerbBases {
    TENSE_MASK    = 0xE000,
    INFINITIVE    = 0x2000,
    PRESENT       = 0x4000,
    PAST          = 0x6000,
    FUTURE        = 0x8000,
    CONTINUOUS        = 0xA000,
    AUXILIAR        = 0xC000,
    INTRANSITIVE        = 0xE000,
    
    MOOD_MASK     = 0x1800,
    INDICATIVE    = 0x0000,
    SUBJUNCTIVE   = 0x0800,
    IMPERATIVE    = 0x1000,
    CONDITIONAL   = 0x1800,
    
    ASPECT_MASK   = 0x0600,
    SIMPLE        = 0x0000,
    PERFECTIVE    = 0x0200,
    IMPERFECTIVE  = 0x0400,
    PROGRESSIVE   = 0x0600, 
    
    VOICE_MASK    = 0x0100,
    ACTIVE        = 0x0000,
    PASSIVE       = 0x0100,

    ANIMACY_MASK  = 0x0080,
    INANIMATE     = 0x0000, 
    ANIMATE_V       = 0x0080,
    
    PERSON_MASK   = 0x0070,
    FIRST_PERSON  = 0x0010,
    SECOND_PERSON = 0x0020,
    THIRD_PERSON  = 0x0030,
    NEGATIVE = 0x0040,
    ZERO_PERSON   = 0x0050,
    
    NUMBER_MASK   = 0x000C,
    SINGULAR      = 0x0004,
    PLURAL_V        = 0x0008,
    DUAL          = 0x000C,
    
    GENDER_MASK   = 0x0003,
    MASCULINE     = 0x0001,
    FEMININE_V      = 0x0002,
    NEUTER        = 0x0003,
};

#define COMBINE(tense, mood, aspect, person, number, gender, voice) \
    ((tense) | (mood) | (aspect) | (person) | (number) | (gender) | (voice))



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
    if (s == "OBLIQUE_PRONOUN") return OBLIQUE_PRONOUN;
    if (s == "PREPOSITION") return PREPOSITION;
    if (s == "ARTICLE") return ARTICLE;
    if (s == "ADVERB") return ADVERB;
    if (s == "POSSESSIVE_PRONOUN") return POSSESSIVE_PRONOUN;

    return UNKNOWN;
}



enum Flags: uint16_t {
    ANIMATE = 1 << 0,      
    NO_PLURAL_ = 1 << 1,  
    IRREGULAR_PLURAL = 1 << 2, 
    IS_PLACE = 1 << 3,    
    ON = 1 << 4,          
    UNCOUNTABLE = 1 << 5, 
    FEMININE_GENDER = 1 << 6, 
    MASCULINE_GENDER = 1 << 7,
    CONJUNCTIVE = 1 << 8, // and
    CONTRASTIVE = 1 << 9, // but
    DISJUNCTIVE = 1 << 10, // or,
    INDEFINITE = 1 << 11,
    PLURAL_NUMBER = 1 << 12,
    NUMBER = 1 << 13,
    NOT_DECIDED_YET = 1 << 14,
    FREE_BIT_2 = 1 << 15
};

#define NEUTRAL_GENDER (FEMININE_GENDER | MASCULINE_GENDER) // haha

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

enum VerbFlags: uint8_t {
    REFLEXIVE_BIT = 0,
    INTRANSITIVE_BIT = 1 << 0,
    DATIVE_CONST = 1 << 1
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
   uint16_t orig_flags; // flags for original word, just remembered that in languages that have same linguistic features but with variation (e.g gender in portuguese and russian) we need to know the flags for both the original word and the translation to make decisions.
   uint16_t flags;
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
inline uint16_t lookupFlags(const Entry (&dict)[N], const char* word) {
    for (size_t i = 0; i < N; ++i) {
        if (strcmp(dict[i].w, word) == 0) {
            return dict[i].flags;
        }
    }
    return 0;
}

inline uint16_t lookupFlags_test(const Entry* dict, size_t count, const char* word) {
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
     uint16_t required_flag;
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
    uint16_t required_flag,
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
        const char* p = dict[i].root;
        if (!p) continue;  
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) return Verb{
            dict[i].root,
            dict[i].translation,
            dict[i].type,      
            dict[i].flags
        };
    }
    return Verb{
            "",       
            "", 
            -1,      
            0
        };
}
inline Verb find_verb_in_array(const Verb dict[], size_t count, const char* word) {
    for (size_t i = 0; i < count; ++i) {
        const char* p = dict[i].root;
        if (!p) continue;
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == '\0' && *q == '\0')
            return dict[i];
    }
    return Verb{"", "", -1, 0};
}


template <size_t N>
inline uint8_t lookupVerbFlags(const Verb (&dict)[N], const char* word) {
    for (size_t i = 0; i < N; ++i) {
        const char* p = dict[i].root;
        if (!p) continue;  
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) return dict[i].flags;
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
        const char* p = dict[i].root;
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

// all the lookups

using Reorder = Vector<Word>(*)(const Vector<Word>&);
using NounLookup = Word(*)(const String&);

//ngram groups
inline String unigramLookup(const Vector<String>& array_of_words,
                                 const Vector<int>& ignore_flags, Reorder reorder_helpers = nullptr, NounLookup nounLookup = nullptr, bool non_spaced = false){

  Vector<Word> sentence_arr;
  Vector<Word> word_arr;

  int match_type;
  String sentence;
  for(size_t i = 0; i < array_of_words.size(); ++i){
    
Word match = nounLookup(array_of_words[i]);
    switch (ignore_flags[i])
    {
    case 0:{
    match_type = match.type;
    if(match.type == -1) match_type = 0;
     String token = array_of_words[i];
    bool isPunct = (token.size() == 1 && (token[0] == '.' || token[0] == '!' || 
                    token[0] == '?' || token[0] == ',' || token[0] == ':' ||
                    token[0] == ';' || token[0] == '(' || token[0] == ')'));
    if (isPunct) {
        match_type = -1;
    }
         Word match_ = {array_of_words[i], match.translation, match_type, match.orig_flags, match.flags};
        sentence_arr.push_back({match.word, match.translation ,match_type, match.orig_flags, match.flags});
        word_arr.push_back(match_);
        break;}
    case 1:{
        Word match_ = {array_of_words[i], array_of_words[i], -1, 0,0};
       sentence_arr.push_back({array_of_words[i], array_of_words[i],-1, 0,0});
         word_arr.push_back(match_);
       break;}
    default:
      break;
    }
  }
  if(word_arr.size() > 0) sentence_arr = reorder_helpers(word_arr);
if(word_arr.size() > 0) {
    sentence_arr = reorder_helpers(word_arr);
}
  
 for (size_t i = 0; i < sentence_arr.size(); ++i) {
    const String& token = sentence_arr.at(i).translation;

    char firstChar = token.empty() ? '\0' : token[0];
    bool isPunctuation = (firstChar == '?' || firstChar == '!' || 
                          firstChar == '.' || firstChar == ','
                          || firstChar == '-' || firstChar == '/' || firstChar == ':');

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




inline bool rule(
       const String& rule,
    const Vector<Word>& sentence_arr,
    Vector<Word>& reordered_arr,
    int i,
    const char* action_arg = nullptr 
) {
    Vector<String> t = parser(rule);
    if (t.size() < 6) return false;
    if (t[0] != "IF") return false;

    String A_str = t[1];
    WordType A = typeFromString(A_str);

    if (t[2] != "THEN") return false;

    struct Matcher { bool isType; WordType type; String literal; };
    Vector<Matcher> Bs;

    int idx = 3;
    {
        WordType wt = typeFromString(t[idx]);
        if (wt == WordType::UNKNOWN)
            Bs.push_back({ false, WordType::UNKNOWN, t[idx] });
        else
            Bs.push_back({ true, wt, "" });
        idx++;
    }

    while (idx < (int)t.size() && t[idx] == "OR") {
        idx++;
        WordType wt = typeFromString(t[idx]);
        if (wt == WordType::UNKNOWN)
            Bs.push_back({ false, WordType::UNKNOWN, t[idx] });
        else
            Bs.push_back({ true, wt, "" });
        idx++;
    }

    Vector<String> actions;
    while (idx < (int)t.size()) {
        if (t[idx] == "DO") {
            idx++;
            if (idx < (int)t.size()) actions.push_back(t[idx]);
            idx++;
        }
        else idx++;
    }

    if (actions.empty()) return false;
    if (i < 1) return false;

    WordType type_prev = static_cast<WordType>(sentence_arr[i - 1].type);
    WordType type_curr = static_cast<WordType>(sentence_arr[i].type);

    String word_curr = sentence_arr[i].word;

    if (type_prev != A) return false;

    bool matchesB = false;
    for (auto& m : Bs) {
        if (m.isType) {
            if (m.type == type_curr) { matchesB = true; break; }
        }
        else {
            if (m.literal == word_curr) { matchesB = true; break; }
        }
    }

    if (!matchesB) return false;

    for (auto& act : actions) {
        auto func = lookupFunction(act.c_str());
        if (func)
            func(reordered_arr, sentence_arr[i], sentence_arr[i - 1], action_arg);
    }

    return true;
}



inline bool default_multibyte = false;
inline bool default_autoCorrect = false;
inline unsigned int ngrams_length = 0;
static inline Entry default_fixed_ngrams[MAX_ENTRIES] = {
  {"", ""}
};

Info default_info = {
    SVO,
    SVO, 
    { 
        { MIDDLE_WORD, 2, POSSESSED_FIRST, INDEFINITE, "" },
    },
    { 
        { MIDDLE_WORD, 0, POSSESSED_FIRST, INDEFINITE, "" },
    },
    1,                
    1,                
    NOUN_FIRST,        
    NOUN_FIRST,
    { 
        
       { PREV_WORD, "", 0, NO_VOWEL_HARMONY },
    }, 
    { 
        { SUFFIX, "", 0, NO_VOWEL_HARMONY }
    },
    1,
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

// whenever parsing the binary, use push_back instead of [i] = value to add new entries.
// unless etl?

inline VerbRuleDictionary default_endings;

inline VerbConjugationDictionary default_conjugations;

MORPH_DEF(default_morph_from,
    {
        {0, "r", "BFV", INFIX, FOCUS_MORPH, NOUN, {NOUN}, STACKABLE, NO_VOWEL_HARMONY},
    });

MORPH_DEF(default_morph_to,
{
        {0, "ес", "BLV", INFIX, FOCUS_MORPH, NOUN, {NOUN}, STACKABLE, NO_VOWEL_HARMONY},
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

inline String normalize(
    String word
) {
    String normalized_ = word;
     for (unsigned int i = 0; i < normalization_ruleCount; ++i)
{
    NORMALIZE(
        default_normalizationRules[i].from,
        default_normalizationRules[i].type,
        default_normalizationRules[i].to
    );
}
    return normalized_;
}

static Vector<Word> default_reorderHelpers(const Vector<Word>& copy){
    Vector<Word> sentence_arr = copy;
    Vector<Word> reordered_arr;

    for (size_t i = 0; i < sentence_arr.size(); ++i) {
     INIT_REORDER()
     DEFAULT()
     
    }
CLEANUP(reordered_arr)



Vector<Word> final_arr;
for (size_t i = 0; i < reordered_arr.size(); ++i) {
    final_arr.push_back(reordered_arr[i]);
}
    return final_arr;
}


static Word default_verb_lookup(const String& word) {
    if (verbs_length == 0 || default_endings.empty() || default_conjugations.empty())
        return Word{word, word, -1};

    bool multibyte = default_multibyte;

    for (const auto& group : default_endings) {
        for (const auto& ending : group.endings) {
            // Check if word ends with this ending
            if (word.size() <= ending.size()) continue;
            
            bool ends_with = false;
            if (multibyte) {
                if (word.compare(word.size() - ending.size(), ending.size(), ending) == 0)
                    ends_with = true;
            } else {
                if (word.compare(word.size() - ending.size(), ending.size(), ending) == 0)
                    ends_with = true;
            }
            if (!ends_with) continue;

            String root = word.substr(0, word.size() - ending.size());

            Verb v = find_verb_in_array(default_verbs, verbs_length, root.c_str());
            if (v.translation && *v.translation) {
                String translation = v.translation;

                for (const auto& conj : default_conjugations) {
                    if (conj.form != group.form) continue;

                    bool condition_met = conj.required_ending.empty();
                    if (!conj.required_ending.empty()) {
                        if (translation.size() >= conj.required_ending.size() &&
                            translation.compare(translation.size() - conj.required_ending.size(),
                                                conj.required_ending.size(),
                                                conj.required_ending) == 0) {
                            condition_met = true;
                            translation = translation.substr(0, translation.size() - conj.required_ending.size());
                        }
                    }

                    if (condition_met) {
                        String result;
                        if (conj.type == PREFIX){
                            String result;
                                result += conj.affix;
                                result += translation;
                       }
                        else // SUFFIX
                           String result;
                            result += translation;
                            result += conj.affix;
                        return Word{word, result, VERB};
                    }
                }
            }
        }
    }
    return Word{word, word, -1};
}


static Word default_nounLookup(const String& word) {
    // Define lookup configurations: {dictionary, length, word_type}
    struct LookupConfig {
        const Entry* dict;
        unsigned int length;
        int type;
    };
    
    const LookupConfig configs[] = {
        {default_nouns, nouns_length, NOUN},
        {default_adjectives, adjective_length, ADJECTIVE},
        {default_pronouns, pronoun_length, PRONOUN},
        {default_adverbs, adverb_length, ADVERB}
    };
    
    // Loop through all word types
    for (const auto& cfg : configs) {
        if (const char* result = lookup_test(cfg.dict, cfg.length, word.c_str())) { 
            String translation = result; 
            int word_type = cfg.type; 
            return { word, normalize(translation), word_type }; 
        }
    }
    
    Word vw = default_verb_lookup(word);
    // verbs return -1 type when no verb is matched
    if (vw.type != -1) return vw;

     /* REGULAR MORPHOLOGICAL DETECTION */                              
       {                                                                   
        const Morph* morph_from_ptr = &(default_morph_from);                   
        String current_word = word;                               
        Vector<const MorphVariation*> applied_morphs;             
        bool found_root = false;                                      
        const char* result = nullptr;                                  
        uint16_t flags = 0;                                            
        int found_type = UNKNOWN;  // Track the type from dictionary lookup
        /* i'll limit it for 10 right now, but when agglutinative languages i'll inflate */ 
        int safety = 10;                                               
        while (!found_root && safety-- > 0) {                          
            MorphResult p = detect_morph(current_word, morph_from_ptr); 
            if (p.matched_variation != nullptr) {                      
                if (p.matched_variation->stackable == STACKABLE) {     
                    /* check if it is stackable*/                      
                    applied_morphs.push_back(p.matched_variation);     
                    current_word = p.root;                             
                    /* Try lookup with stripped word */                
                    // Check against all dictionaries
                    for (const auto& cfg : configs) {
                        if ((result = lookup_test(cfg.dict, cfg.length, current_word.c_str()))) {
                            flags = lookupFlags_test(cfg.dict, cfg.length, current_word.c_str());
                            found_type = cfg.type;
                            found_root = true;
                            break;
                        }
                    }
                    if (found_root) break;
                } else {                                               
                    /* Non-stackable, just get it and stop */          
                    applied_morphs.push_back(p.matched_variation);     
                    // Check against all dictionaries
                    for (const auto& cfg : configs) {
                        if ((result = lookup_test(cfg.dict, cfg.length, p.root.c_str()))) {
                            flags = lookupFlags_test(cfg.dict, cfg.length,p.root.c_str());
                            found_type = cfg.type;
                            found_root = true;
                            break;
                        }
                    }
                    break;
                }                                                      
            } else {                                                   
                break;                                                 
            }                                                          
        }                                                              
        if (found_root) {                                              
            String translation = result;                          
            const Morph* morph_to_ptr = &(default_morph_to);                   
            for (auto it = applied_morphs.rbegin(); it != applied_morphs.rend(); ++it) { 
                translation = apply_morph(translation, *it, morph_to_ptr, flags); 
            }                                                          
            return { word, normalize(translation), found_type, 0, flags };   
        }                                                              
    }    


    //fallback, words with no matching at all return type -1 unchanged strings
    return Word{ word, normalize(word), -1 };  

}
// mapping out how i'm gonna receive the binary file buffers to dinamically define the rules 
// it works!!
inline String load_from_bin(const uint8_t* file, size_t size)
{
    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(file);
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
    
    for (int i = 0; i < MAX_NORMALIZATION_RULES; i++) {
        new (&default_normalizationRules[i]) NormalizationRule();
    new (&default_normalizationRules[i].from) String();
    new (&default_normalizationRules[i].to) String();
    }
    for (int i = 0; i < MAX_ENTRIES; i++) {
        new (&default_nouns[i]) Entry();
        new (&default_adjectives[i]) Entry();
        new (&default_adverbs[i]) Entry();
        new (&default_verbs[i]) Verb();
        if (i < 20) new (&default_pronouns[i]) Entry();
    }
    
    // Clear to initial state
    default_endings.clear();
    default_conjugations.clear();
    temp_verb_endings.clear();
    temp_required_ending.clear();
    temp_type = 0;
    normalizationRuleLength = 0;
#else
    // STL: just clear
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
    
    static uint8_t target_flags = 0;

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
    
         default_conjugations.push_back(conj);
    }
     else if (current_area == 5) {
    if (s && r) {
       if (normalizationRuleLength < MAX_NORMALIZATION_RULES) {
            default_normalizationRules[normalizationRuleLength++] = 
                NormalizationRule{String(s), String(r), type};
        } 
    }
    s = nullptr;
    r = nullptr;
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
        //read the word flags
        uint16_t flag_count = *ptr++;
        flag_count |= (*ptr++) << 8;
        
        target_flags = 0;
        
        for (int i = 0; i < flag_count; i++) {
            uint8_t flag_value = *ptr++;
            target_flags |= flag_value;
         }
        
        // acctually push it into the arrays based on ngram_size
        switch (ngram_size)
        {
            case 0x01:
                if(type == 0x00) {
                    Entry e = {s, r, 0, target_flags};
                    default_nouns[nouns_length++] = e;
                }
                if(type == 0x01) {
                    Entry e = {s, r, 0, target_flags};
                    default_adjectives[adjective_length++] = e;
                 }
                if(type == 0x03) {
                    Verb e = {s, r, 0, target_flags};
                    default_verbs[verbs_length++] = e;
                 }
                if(type == 0x04) {
                    Entry e = {s, r, 0, target_flags};
                    default_pronouns[pronoun_length++] = e;
                 }
                      if(type == 0x08) {
                    Entry e = {s, r, 0, target_flags};
                    default_adverbs[adverb_length++] = e;
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
        uint16_t flag_count = *ptr++;
        flag_count |= (*ptr++) << 8;
        
        target_flags = 0;
        
        for (int i = 0; i < flag_count; i++) {
            uint8_t flag_value = *ptr++;
            target_flags |= flag_value;
        }
        
        switch (ngram_size)
        {
            case 0x01:
                if(type == 0x00) {
                    Entry e = {s, r, 0, target_flags};
                    default_nouns[nouns_length++] = e;
                }
                if(type == 0x01) {
                    Entry e = {s, r, 0, target_flags};
                    default_adjectives[adjective_length++] = e;
                }
                if(type == 0x04) {
                    Entry e = {s, r, 0, target_flags};
                    default_pronouns[pronoun_length++] = e;
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

default:
    // Handle unknown markers
    break;

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
return String("Loaded translator: "/**+ from + " > " + to + "\n" */ );
}
inline String translate_from_bin(const char* sentence,
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
               if (lookup_test(default_nouns, nouns_length, cand.c_str()) ||
                    lookup_test(default_adjectives, adjective_length, cand.c_str()) ||
                    lookup_test(default_pronouns, pronoun_length, cand.c_str()) ||
                    lookup_test(default_adverbs, adverb_length, cand.c_str())
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
