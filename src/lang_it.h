#ifndef LANG_IT_H
#define LANG_IT_H


#include <string>
#include <cstdint>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cstddef>
#include <iostream>

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
    REDUPLICATION = 6,
    MIDDLE_WORD = 7,
    NONE = 8
};
#define LOOKUP(DICTIONARY, TYPE, WORD, GENDER_FROM, GENDER_TO, PLURAL_FROM, PLURAL_TO)          \
{                                                                       \
    GenderResult g = detect_gender(WORD, GENDER_FROM);                  \
    MorphResult p = detect_plural(WORD, PLURAL_FROM); \
            /*DEFAULT */         \
    if (const char* result = lookup(DICTIONARY, WORD.c_str())) {                \
        uint16_t flags = lookupFlags(DICTIONARY, WORD.c_str()); \
        return { WORD, normalize(result), TYPE, 0, flags };                       \
    }                             \
                 /*GENDER BUT SINGULAR */                               \
    if (const char* result = lookup(DICTIONARY, g.root.c_str())) {      \
        uint16_t flags = lookupFlags(DICTIONARY, g.root.c_str()); \
        std::string translation = result;                                \
        translation = apply_gender(translation, g.matched_variation, GENDER_TO); \
        return { WORD, normalize(translation), TYPE, 0, flags };                   \
    }                                                                   \
        /* ALL KINDS OF MORPHS LOL*/  \
    if (const char* result = lookup(DICTIONARY, p.root.c_str())) {      \
        uint16_t flags = lookupFlags(DICTIONARY, p.root.c_str()); \
        std::string translation = result;                                \
        translation = apply_morph(translation, p.matched_variation, PLURAL_TO, flags); \
        return { WORD, normalize(translation), TYPE, 0, flags };                   \
    }  \
    if(WORD.length() > 3){\
       if (auto_correct(DICTIONARY, WORD)) {                \
        uint16_t flags = lookupFlags(DICTIONARY, auto_correct(DICTIONARY, WORD)); \
        return { WORD, normalize(auto_correct(DICTIONARY, WORD)), TYPE, 0, flags };                       \
    }\
}\
}

#define NO_GENDER (Gender*)nullptr
#define NO_PLURAL (Morph*)nullptr
#define NO_CASE (Case*)nullptr


enum SCRIPTS {
    LATIN = 0,
    CYRILIC = 2,
    HAN = 3,
    KANA = 4,
    ARABIC = 5,
    PERSIAN = 6,
    HANGUL = 7
};



typedef struct  {
   int type; // how does it act?
   int word; // what word does it act upon, A or B?
   int order; // A * B or B * A?
   uint8_t flags;
   std::string addition; 

} GenitiveConstruction;

enum AdjectiveOrder {
   ADJECTIVE_FIRST = 0,
   NOUN_FIRST = 1
};

typedef struct {
   int clause_order_from;
   int clause_order_to;
   
   GenitiveConstruction gc_from[10]; 
   GenitiveConstruction gc_to[10];   
   AdjectiveOrder ao_from;
   AdjectiveOrder ao_to;
   int gc_from_count;                
   int gc_to_count;  
   
   int scripts_from[4];
   int scripts_to[4];
} Info;


inline size_t utf8_length(const std::string& str) {
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

inline bool ends_with_multibyte(const std::string& str, const std::string& suffix) {
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
    
    return str.compare(byte_pos, std::string::npos, suffix) == 0;
}

inline std::string remove_ending_multibyte(const std::string& str, const std::string& suffix) {
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
#define VERB_LOOKUP(DICTIONARY, WORD, ENDINGS, CONJUGATIONS, MORPH_FROM, MORPH_TO, MULTIBYTE)    \
{                                                                           \
    if (MULTIBYTE) {                                                        \
        /* Multibyte-aware version */                                      \
        for (size_t ci = 0; ci < ENDINGS.size(); ++ci) {                   \
            for (size_t ei = 0; ei < ENDINGS[ci].endings.size(); ++ei) {   \
                const std::string& ending = ENDINGS[ci].endings[ei];       \
                int ending_form = ENDINGS[ci].form;                        \
                /* Check if word ends with this ending (multibyte aware) */ \
                if (!ends_with_multibyte(WORD, ending)) {                  \
                     continue;                                              \
                }                                                          \
                                                                           \
                /* Get root by removing ending (multibyte aware) */       \
                std::string root = remove_ending_multibyte(WORD, ending);  \
                Verb v = verb_lookup(DICTIONARY, root.c_str());           \
                if (v.translation && *v.translation) {                    \
                    std::string translation = v.translation;              \
                    std::string prefix = "";                              \
                    std::string suffix = "";                              \
                    bool conjugation_applied = false;                     \
                                                                           \
                    for (const auto& conj : CONJUGATIONS) {               \
                        if (conj.form == ending_form) {                   \
                            bool condition_met = conj.required_ending.empty() || \
                                ends_with_multibyte(translation, conj.required_ending); \
                                                                           \
                            if (condition_met) {                          \
                               std::string stem = translation;          \
                                if (!conj.required_ending.empty()) {      \
                                    stem = remove_ending_multibyte(translation, \
                                                                  conj.required_ending); \
                                }                                         \
                                                                           \
                                if (conj.type == PREFIX) {                \
                                    prefix = conj.affix;                  \
                                 } else {                                   \
                                    suffix = conj.affix;                  \
                                }                                          \
                                translation = stem;                       \
                                conjugation_applied = true;               \
                                break;                                     \
                            } else {                                       \
                            }                                              \
                        }                                                  \
                    }                                                      \
                     return Word{ WORD, prefix + translation + suffix, VERB }; \
                } else {                                                   \
                }                                                          \
            }                                                              \
        }                                                                  \
    } else {                                                                                   \
        for (size_t ci = 0; ci < ENDINGS.size(); ++ci) {                  \
            for (size_t ei = 0; ei < ENDINGS[ci].endings.size(); ++ei) {  \
                const std::string& ending = ENDINGS[ci].endings[ei];      \
                int ending_form = ENDINGS[ci].form;                       \
                if (WORD.size() <= ending.size()) continue;               \
                 if (WORD.compare(WORD.size() - ending.size(),             \
                                  ending.size(), ending) != 0) {          \
                    continue;                                             \
                }                                                         \
                   std::string root = WORD.substr(0, WORD.size() - ending.size());\
                                                                            \
                Verb v = verb_lookup(DICTIONARY, root.c_str());          \
                if (v.translation && *v.translation) {                   \
                     std::string translation = v.translation;             \
                    std::string prefix = "";                             \
                    std::string suffix = "";                             \
                    bool conjugation_applied = false;                    \
                                                                           \
                    for (const auto& conj : CONJUGATIONS) {              \
                        if (conj.form == ending_form) {                  \
                             bool condition_met = conj.required_ending.empty() || \
                                (translation.size() >= conj.required_ending.size() && \
                                 translation.compare(translation.size() - conj.required_ending.size(), \
                                                    conj.required_ending.size(), \
                                                    conj.required_ending) == 0); \
                                                                           \
                            if (condition_met) {                         \
                                  std::string stem = translation;         \
                                if (!conj.required_ending.empty()) {    \
                                    stem = translation.substr(0,       \
                                        translation.size() - conj.required_ending.size()); \
                                 }                                        \
                                                                           \
                                if (conj.type == PREFIX) {              \
                                    prefix = conj.affix;                \
                                } else {                                 \
                                    suffix = conj.affix;                \
                                }                                        \
                                translation = stem;                     \
                                conjugation_applied = true;              \
                                break;                                   \
                            } else {                                     \
                            }                                            \
                        }                                                \
                    }                                                    \
                                                                           \
                     return Word{ WORD, prefix + translation + suffix, VERB }; \
                } else {                                                   \
                 }                                                          \
            }                                                            \
        }                                                                \
    }                                                                    \
   return Word{ WORD, WORD, -1 };                                       \
}

   #define SUFFIX_LOOKUP(DICTIONARY, WORD, ADJECTIVES)                         \
{                                                                           \
 std::string translation;\
 int word_type = 0;\
  for (int len = 6; len >= 2; --len) {\
    if (WORD.length() >= static_cast<size_t>(len)) {\
        std::string ending = WORD.substr(WORD.length() - len);\
        Suffix suffResult = lookupSuff(DICTIONARY, ending.c_str());\
        if (suffResult.t) {\
            const char* mapped = suffResult.t;\
            std::string stem = WORD.substr(0, WORD.length() - len);\
            word_type = suffResult.type;\
            const char* adjResult = lookup(ADJECTIVES, stem.c_str());\
            if (adjResult) {\
                translation = std::string(adjResult) + mapped;\
            word_type = suffResult.type;\
            }\
            else if (!stem.empty()) {\
                std::string altStem = stem.substr(0, stem.length() - 1) + "o";\
                const char* altAdj = lookup(ADJECTIVES, altStem.c_str());\
                if (altAdj)\
                    translation = std::string(altAdj) + mapped;\
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
        const std::string& orig = ORIGINAL;                         \
        const std::string& repl = REPLACEMENT;                      \
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


    



// pairs

#if defined(PT_EN) || defined(ALL) 
std::string traduzir_en(const char* sentence, bool auto_correct);
#endif

#if defined(PT_MBL) || defined(ALL) 
std::string pt_mbl(const char* sentence);
#endif

#if defined(JA_MBL) || defined(ALL) 
std::string ja_mbl(const char* sentence);
#endif


#if defined(AR_TH) || defined(ALL) 
std::string ar_th(const char* sentence);
#endif


#if defined(PT_RU) || defined(ALL) 
std::string pt_ru(const char* sentence);
#endif

#if defined(PT_JA) || defined(ALL) 
std::string pt_ja(const char* sentence);
#endif

#if defined(AA_BB) || defined(ALL) 
std::string aa_bb(const char* sentence);
#endif

#if defined(PT_ES) || defined(ALL)
std::string pt_es(const char* sentence);
#endif

#if defined(EN_JA) || defined(ALL)
std::string translate_ja(const char* sentence, int script);
#endif

#if defined(MA_ZH) || defined(ALL)
std::string ma_zh(const char* sentence);
#endif


#if defined(EN_PT) || defined(ALL)
std::string translate_pt(const char* sentence);
#endif

#if defined(PT_SV) || defined(ALL)
std::string traduzir_sv(const char* sentence);
#endif


#if defined(ZH_EN) || defined(ALL)
std::string translate_zh(const char* sentence);
#endif


/* ------- GLOBAL CORE FUNCTIONS -----------
|           all pairs use                  |  
------------------------------------------*/



typedef struct {
    const char* w;
    const char* t;
    uint16_t orig_flags; // flags for original word, just remembered that in languages that have same linguistic features but with variation (e.g gender in portuguese and russian) we need to know the flags for both the original word and the translation to make decisions.
    uint16_t flags;
} Entry;

struct Verb {
    const char* root;       
    const char* translation; 
    int type;      
    uint8_t flags;
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
    std::string ending;
    std::string form;
};

struct Case {
    int type; 
    std::vector<CaseVariation> variations;
};

struct CaseResult {
    std::string root;
    const CaseVariation* matched_variation; 
};


struct GenderVariation {
    uint8_t flag;
    std::string form;
};

struct Gender {
    int type; 
    std::vector<GenderVariation> variations;
};

struct GenderResult {
    std::string root;
    const GenderVariation* matched_variation; 
};

enum Morphology {
   PLURAL_MORPH = 0,
   DIMINUTIVE_MORPH = 1,
   AUGMENTATIVE_MORPH = 2,
   COMPARATIVE_MORPH = 3,
   SUPERLATIVE_MORPH = 4,
   DEFINITE_MORPH = 5,
   AGENT_MORPH = 6

};


struct MorphVariation {
    uint8_t flag;
    std::string ending;
    std::string form;
    int type;
    int morphology;
};

struct Morph {
    std::vector<MorphVariation> variations;
};
struct MorphResult {
    std::string root;
    const MorphVariation* matched_variation; 
};


inline CaseResult detect_case(const std::string& word, const Case* case_from) {
    if (!case_from) {
        return { word, nullptr };
    }

    for (const auto& var : case_from->variations) {
        const std::string& suffix = var.form;

        if (word.size() >= suffix.size() &&
            word.compare(word.size() - suffix.size(), suffix.size(), suffix) == 0)
        {
            std::string root = word.substr(0, word.size() - suffix.size());
            return { root, &var };
        }
    }
    return { word, nullptr };
}


inline std::string apply_case(
    const std::string& translation,
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
            const std::string& ending = var.ending;
            const std::string& form   = var.form;

            if (translation.size() >= ending.size() &&
                translation.compare(translation.size() - ending.size(), ending.size(), ending) == 0) 
            {
                return (translation.substr(0, translation.size() - ending.size()) + form);
            }
        }
    }
}

    for (const auto& var : case_to->variations) {
        if (word_gender == 0 || (var.gender & word_gender)) {
            const std::string& ending = var.ending;
            const std::string& form   = var.form;

            if (ending.length() <= translation.length())
                return translation.substr(0, translation.length() - ending.length()) + form;
            else
                return form;
        }
    }
    }else if (from_var && !case_to) {
    // CASE → NON-CASE
    }else if (from_var && case_to) {
    // CASE → CASE
}



    // Final fallback: just use the first variation
    const auto& var = case_to->variations[0];
    if (var.ending.length() <= translation.length())
        return translation.substr(0, translation.length() - var.ending.length()) + var.form;
    else
        return var.form;
}

inline GenderResult detect_gender(const std::string& word, const Gender* gender_from) {
    if (!gender_from) {
        return { word, nullptr };
    }

    for (const auto& var : gender_from->variations) {
        const std::string& suffix = var.form;

        if (word.size() >= suffix.size() &&
            word.compare(word.size() - suffix.size(), suffix.size(), suffix) == 0)
        {
            std::string root = word.substr(0, word.size() - suffix.size());
            return { root, &var };
        }
    }
    return { word, nullptr };
}


inline MorphResult detect_plural(const std::string& word, const Morph* plural_from) {
    if (!plural_from) {
        return { word, nullptr };
    }

    for (const auto& var : plural_from->variations) {
        const std::string& plural_ending = var.ending; 
        const std::string& singular_ending = var.form;    
        
        if (word.size() >= plural_ending.size() &&
            word.compare(word.size() - plural_ending.size(), plural_ending.size(), plural_ending) == 0)
        {
            std::string base = word.substr(0, word.size() - plural_ending.size());
            std::string root = base + singular_ending;
            return { root, &var };
        }
    }
    
    return { word, nullptr };
}




inline std::string apply_gender(
    const std::string& translation,
    const GenderVariation* from_var,
    const Gender* gender_to)
{
    if (!gender_to)
        return translation;

    // NON → GENDERED
    if (!from_var) {
        if (!gender_to->variations.empty())
            return translation + gender_to->variations[0].form;
        return translation;
    }

    // GENDERED → GENDERED
    for (const auto& to_var : gender_to->variations) {
        if (to_var.flag == from_var->flag)
            return translation + to_var.form;
    }

    // fallback
    return translation;
}


inline std::string apply_morph(
    const std::string& translation,
    const MorphVariation* from_var,
    const Morph* morph_to,
    uint8_t word_gender)
{
    if (!morph_to || !from_var)
        return translation;

    int source_morph = from_var->morphology; 

    for (const auto& var : morph_to->variations) {
        if (var.morphology == source_morph) { 
            
            // Check flags if needed
            if ((var.flag & word_gender) == var.flag) {
                const std::string& ending = var.ending;
                const std::string& form   = var.form;
                
                switch(source_morph){
                case SUFFIX:
                  if (translation.size() >= ending.size() &&
                    translation.compare(translation.size() - ending.size(), ending.size(), ending) == 0) 
                {
                    // what type of transformation are we doing
                        switch (var.type) {
                            case SUFFIX:
                                if (translation.size() >= ending.size() &&
                                    translation.compare(translation.size() - ending.size(), ending.size(), ending) == 0) 
                                {
                                    return translation.substr(0, translation.size() - ending.size()) + form;
                                }
                                break;
                                
                            case PREFIX:
                                if (translation.size() >= ending.size() &&
                                    translation.compare(0, ending.size(), ending) == 0)
                                {
                                    return translation.substr(ending.size()) + form;
                                }
                                break;
                                
                            case PREV_WORD:
                                return form + translation;  // "大" + "犬", "big" + "dog"
                                
                            case NEXT_WORD:
                                return translation + form;  // "犬" + "たち",
                                
                            // etc.
                        }}
                break;
                case PREV_WORD:
                if (translation.size() >= ending.size() &&
                    translation.compare(translation.size() - ending.size(), ending.size(), ending) == 0)
                break;
            }
        }
        }
    }

    // fallback
    return translation;
}
typedef struct 
{
    std::vector<std::string> endings;
    int type;
    int form;
} VerbRule;

typedef struct 
{
    int form;     
    int type;       
    std::string required_ending;
    std::string affix;
} VerbConjugation;

using Dictionary = Entry[];
using VerbDictionary = Verb[];
using SuffixDictionary = Suffix[];
using VerbRuleDictionary = std::vector<VerbRule>;
using VerbConjugationDictionary = std::vector<VerbConjugation>;



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
#define LIST(name, ...) std::vector<SuffixRule> name = __VA_ARGS__

#define MORPH_DEF(name,...) Morph name = {__VA_ARGS__}
#define GENDER_DEF(name,...) Gender name = {__VA_ARGS__}
#define CASE_DEF(name,...) Case name = {__VA_ARGS__}


#define VERB_ENDINGS(name, ...) inline VerbRuleDictionary name = __VA_ARGS__

#define VERB_CONJUGATIONS(name, ...) inline VerbConjugationDictionary  name = __VA_ARGS__




#define COMBINE_VERB_TOKENS(INPUT, OUTPUT, VERB_DICT, ENDINGS, MAX_STEM, MAX_ENDING) \
    do { \
        std::vector<std::string> temp; \
        size_t i = 0; \
        while (i < (INPUT).size()) { \
            bool found = false; \
            for (size_t stem_len = (MAX_STEM); stem_len >= 1; --stem_len) { \
                if (i + stem_len > (INPUT).size()) continue; \
                std::string stem_candidate; \
                for (size_t k = 0; k < stem_len; ++k) \
                    stem_candidate += (INPUT)[i + k]; \
                Verb v = verb_lookup((VERB_DICT), stem_candidate.c_str()); \
                if (v.translation && *v.translation) { \
                    for (size_t ending_len = (MAX_ENDING); ending_len >= 1; --ending_len) { \
                        if (i + stem_len + ending_len > (INPUT).size()) continue; \
                        std::string ending_candidate; \
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
        std::vector<std::string> temp; \
        size_t i = 0; \
        while (i < (INPUT).size()) { \
            bool found = false; \
            for (size_t stem_len = (MAX_STEM); stem_len >= 1; --stem_len) { \
                if (i + stem_len > (INPUT).size()) continue; \
                std::string stem_candidate; \
                for (size_t k = 0; k < stem_len; ++k) \
                    stem_candidate += (INPUT)[i + k]; \
                Verb v = find_verb_in_array((VERB_DICT), verbs_length, stem_candidate.c_str());\
                if (v.translation && *v.translation) { \
                    for (size_t ending_len = (MAX_ENDING); ending_len >= 1; --ending_len) { \
                        if (i + stem_len + ending_len > (INPUT).size()) continue; \
                        std::string ending_candidate; \
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


#define COMBINE_VERB_STEMS(INPUT, OUTPUT, VERB_DICT, ENDINGS)              \
    do {                                                                    \
        std::vector<Word> temp;                                             \
        for (size_t i = 0; i < (INPUT).size(); ++i) {                      \
            bool combined = false;                                          \
            Verb v = verb_lookup(VERB_DICT, (INPUT)[i].word.c_str());      \
            if (v.translation && *v.translation) {                         \
               if (i + 1 < (INPUT).size()) {                              \
                    const std::string& next = (INPUT)[i+1].word;           \
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
std::string name(const char* sentence) { \
    char buffer[250]; \
    strncpy(buffer, sentence, sizeof(buffer)); \
    buffer[sizeof(buffer) - 1] = '\0'; \
    to_lower(buffer); \
    std::vector<std::string> arr; \
    if(MULTIBYTE == true) { \
        arr = tokenize_cjk(std::string(buffer)); \
        /* 1. Combine verb stems + endings */ \
        std::vector<std::string> verb_combined; \
        COMBINE_VERB_TOKENS(arr, verb_combined, verbs, VERB_ENDINGS, 3, 2); \
        std::vector<std::string> final_combined; \
        size_t i = 0; \
        while (i < verb_combined.size()) { \
            bool found = false; \
            for (size_t len = 4; len >= 1; --len) { \
                if (i + len > verb_combined.size()) continue; \
                std::string cand; \
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
        arr = tokenize(std::string(buffer)); \
    } \
    std::string translated = trigramLookup(NGRAMS, arr, REORDER_HELPERS, LOOKUP_FUNCTION, SPACED); \
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
            if (current.type == 3 && (next.type == 0 || next.type == 4 || next.type == -1)) { \
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
#define HANDLE_POSSESSION(INFO_ARG, ARR) \
    do { \
                if ((ARR).size() >= 2) { \
                    std::vector<Word> replacement; \
                    AdjectiveOrder ao_from = (INFO_ARG)->ao_from; \
                    AdjectiveOrder ao_to = (INFO_ARG)->ao_to; \
                      for (size_t i = 0; i + 1 < (ARR).size(); ++i) { \
                        if(ao_from != ao_to && (((ARR)[i].type == NOUN &&  (ARR)[i + 1].type == ADJECTIVE) || ((ARR)[i].type == NOUN &&  (ARR)[i + 1].type == ADJECTIVE))){\
                          Word temp_word_i = (ARR)[i];\
                          Word temp_word_i_ = (ARR)[i + 1];\
                          (ARR)[i] = temp_word_i_;\
                          (ARR)[i + 1] = temp_word_i;\
                        }\
                      }\
                }\
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
                        idxA = i;      /* A is first */ \
                        idxB = i + 2;  /* B is last */ \
                    } else { /* OWNER_FIRST */ \
                        idxA = i + 2;  /* A is last */ \
                        idxB = i;      /* B is first */ \
                    } \
                    Word A = (ARR)[idxA]; \
                    Word B = (ARR)[idxB]; \
                    \
                    /* Apply target construction (use first gc_to for simplicity) */ \
                    if ((INFO_ARG)->gc_to_count == 0) break; \
                    GenitiveConstruction gc_to = (INFO_ARG)->gc_to[0]; \
                    \
                    /* Build replacement words according to target order */ \
                    std::vector<Word> replacement; \
                    if (gc_to.order == POSSESSED_FIRST) { \
                        replacement.push_back(Word{A.word, A.translation, A.type, A.orig_flags, A.flags}); \
                        replacement.push_back(Word{gc_to.addition, gc_to.addition, 5, 0, 0}); /* particle type */ \
                        replacement.push_back(Word{B.word, B.translation, B.type, B.orig_flags, B.flags}); \
                    } else { /* OWNER_FIRST */ \
                        replacement.push_back(Word{B.word, B.translation, B.type, B.orig_flags, B.flags}); \
                        replacement.push_back(Word{gc_to.addition, gc_to.addition, 5, 0, 0}); \
                        replacement.push_back(Word{A.word, A.translation, A.type, A.orig_flags, A.flags}); \
                    } \
                    \
                    /* Replace the three old words with the new ones */ \
                    (ARR).erase((ARR).begin() + i, (ARR).begin() + i + 3); \
                    (ARR).insert((ARR).begin() + i, replacement.begin(), replacement.end()); \
                    \
                    found = true; \
                    break; /* exit inner loop */ \
                } \
                if (found) break; /* exit pattern loop after first replacement */ \
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
    // TENSE (bits 15-13) - 8 values
    TENSE_MASK    = 0xE000,
    INFINITIVE    = 0x2000,
    PRESENT       = 0x4000,
    PAST          = 0x6000,
    FUTURE        = 0x8000,
    CONTINUOUS        = 0xA000,
    TENSE6        = 0xC000,
    TENSE7        = 0xE000,
    
    // MOOD (bits 12-11) - 4 values
    MOOD_MASK     = 0x1800,
    INDICATIVE    = 0x0000,
    SUBJUNCTIVE   = 0x0800,
    IMPERATIVE    = 0x1000,
    CONDITIONAL   = 0x1800,
    
    // ASPECT (bits 10-9) - 4 values
    ASPECT_MASK   = 0x0600,
    SIMPLE        = 0x0000,
    PERFECTIVE    = 0x0200,
    IMPERFECTIVE  = 0x0400,
    PROGRESSIVE   = 0x0600,  // Replaced HABITUAL
    
    // VOICE (bit 8) - 2 values (now a single bit)
    VOICE_MASK    = 0x0100,
    ACTIVE        = 0x0000,
    PASSIVE       = 0x0100,
    // MIDDLE and CAUSATIVE can be handled by required_ending or separate constructions
    
    // ANIMACY (bit 7) - 2 values (NEW!)
    ANIMACY_MASK  = 0x0080,
    INANIMATE     = 0x0000,  // Default
    ANIMATE_V       = 0x0080,
    
    // PERSON (bits 6-4) - 8 values (still 5 used, 3 free)
    PERSON_MASK   = 0x0070,
    FIRST_PERSON  = 0x0010,
    SECOND_PERSON = 0x0020,
    THIRD_PERSON  = 0x0030,
    FOURTH_PERSON = 0x0040,
    ZERO_PERSON   = 0x0050,
    // 0x0060 and 0x0070 are FREE for future use
    
    // NUMBER (bits 3-2) - 4 values
    NUMBER_MASK   = 0x000C,
    SINGULAR      = 0x0004,
    PLURAL_V        = 0x0008,
    DUAL          = 0x000C,
    
    // GENDER (bits 1-0) - 4 values
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



inline WordType typeFromString(const std::string& s) {
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
    NEUTRAL_GENDER = 1 << 7,
    CONJUNCTIVE = 1 << 8, // and
    CONTRASTIVE = 1 << 9, // but
    DISJUNCTIVE = 1 << 10, // or,
    INDEFINITE = 1 << 11,
    FREE_BIT_1 = 1 << 12,
    FREE_BIT_2 = 1 << 13
};

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
    PLURAL = 0,
    FEMININE = 1 << 0
};

typedef struct {
    
    std::string root;
    std::vector<std::string> suffixes;
} SuffixRule;

typedef struct {
  std::string original;
  std::string replacement;
} Normalization;


typedef struct {
   std::string word;
   std::string translation;
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
std::vector<Word> MEDIATE_HOMONYMS(
    std::vector<Word> arr,
    const std::vector<std::string>& words,
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

                std::vector<std::string> context;
                for (int j = start; j <= end; ++j) {
                    context.push_back(arr[j].translation);
                }

                size_t contextIndex = static_cast<size_t>(i - start);

                std::vector<std::string> this_context = context;
                std::vector<int> word_types(context.size(), 0);

                this_context[contextIndex] = arr[i].word;
                word_types[contextIndex]   = arr[i].type;

                std::string resolved_word =
                    semantics(this_context,
                              word_types,
                              contextIndex,
                              homonyms,   
                              N);        

                arr[i].translation = resolved_word;
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

inline std::vector<Word> INSERT_PARTICLE(
    std::vector<Word> arr,
    int type_before,
    int type_after,
    const char* particle_word,
    bool sentence_size,
    uint16_t required_flag,
    FlagTarget flag_target, 
    const char* particle_translation = nullptr,
    int particle_type = -1
) {
    std::vector<Word> result;
    
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

inline std::vector<Word> INSERT_PARTICLES(
    std::vector<Word> arr, 
    const std::vector<ParticleRule>& rules
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


inline std::vector<Word> POST_CONJUGATION(
    std::vector<Word> arr
   
) {
    std::vector<Word> result;
    
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
inline const char* lookup(const Entry (&dict)[N], const char* word) {
    for (size_t i = 0; i < N; ++i) {
        const char* p = dict[i].w;
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) return dict[i].t;
    }
    return nullptr;
}

inline const char* lookup_test(const Entry* dict, size_t count, const char* word)
{
    for (size_t i = 0; i < count; ++i)
    {
        const char* p = dict[i].w;
        const char* q = word;

        if (!p) continue;

        while (*p && *q && *p == *q)
        {
            ++p;
            ++q;
        }

        if (*p == '\0' && *q == '\0')
            return dict[i].t;
    }
    return nullptr;
}


template <size_t N>
inline const char* auto_correct(const Entry(&dict)[N] , std::string word) {
   std::string result = word;
   std::string word_temp = word;
   
   //try different letters at every position (femilia -> familia)
   std::string possible_letters = "abcdefghjijklmnopqrstuvwxyz";
   for(size_t i = 0; i < word.length(); i++){
    for(size_t j = 0; j < possible_letters.length(); j++){
      word_temp.replace(i, 1, 1, possible_letters[j]);
      const char* lookup_ = lookup(dict, word_temp.c_str());
      if(lookup_ && std::string(lookup_).length() > 2) {
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
      if(lookup_ && std::string(lookup_).length() > 2) {
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
            if(lookup_ && std::string(lookup_).length() > 2) {
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
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) return dict[i].flags;
    }
    return 0;
}





//invert a pair (casa azul -> azul casa)
inline void invert(std::vector<Word>& output, const Word& first, const Word& second, const char*) {
    
    if (!output.empty()) output.pop_back(); 
    output.push_back(first);
    output.push_back(second);
}
//
inline void replace_first(std::vector<Word>& output, const Word& replacement, const Word& second, const char*) {
    if (!output.empty()) output.pop_back();

    output.push_back(replacement);
    output.push_back(second);
}

// inject a word in the middle of two words (orange juice -> suco de laranja)
inline void sandwich(std::vector<Word>& output, const Word& first, const Word& second, const char* word) {
    if (!output.empty()) output.pop_back(); 
    output.push_back(first);
    output.push_back(Word{word, word, 0});
    output.push_back(second);
}

// replace a word in the middle of two words (orange juice -> suco de laranja)
inline void sandwich_replace(std::vector<Word>& output,const Word& middle, const Word& second, const char*) {
    if (!output.empty()) output.pop_back(); 
    output.push_back(middle);
    output.push_back(second);
}

//remove middle word
inline void remove_middle(std::vector<Word>& output, const Word& first, const Word& middle, const Word& last) {
    if (!output.empty()) output.pop_back();
    output.push_back(first);
    output.push_back(last);
}
inline void remove_first(std::vector<Word>& output, const Word& first, const Word& second, const char*) {
  if (!output.empty()) {output.pop_back(); }
    output.push_back(first);
}

inline void remove_current(std::vector<Word>& output) {
  if (output.size() > 1) {
        output.erase(output.end() - 1, output.end()); 
    }           
}

inline void remove_pair(std::vector<Word>& output) {
    if (output.size() >= 2) {
        output.erase(output.end() - 2, output.end()); 
    }
}

// all the lookups

using Reorder = std::vector<Word>(*)(const std::vector<Word>&);
using NounLookup = Word(*)(const std::string&);

//ngram groups
inline std::string unigramLookup(const std::vector<std::string>& array_of_words,
                                 const std::vector<int>& ignore_flags, Reorder reorder_helpers = nullptr, NounLookup nounLookup = nullptr, bool non_spaced = false){

  std::vector<Word> sentence_arr;
  std::vector<Word> word_arr;

  int match_type;
  std::string sentence;
  for(size_t i = 0; i < array_of_words.size(); ++i){
    
Word match = nounLookup(array_of_words[i]);
    switch (ignore_flags[i])
    {
    case 0:{
    match_type = match.type;
    if(match.type == -1) match_type = 0;
     std::string token = array_of_words[i];
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
    const std::string& token = sentence_arr.at(i).translation;

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
inline std::string bigramLookup(const Entry (&fixed_ngrams)[N],
                                const std::vector<std::string>& words,
                                std::vector<int>& ignore_flags, Reorder reorder_helpers = nullptr, NounLookup nounLookup = nullptr, bool non_spaced = false) {
    std::vector<std::string> mended_array_of_words;
    std::vector<int> new_ignore_flags;

    size_t i = 0;
    while (i < words.size()) {
        if (i + 1 < words.size() && ignore_flags[i] == 0 && ignore_flags[i + 1] == 0) {
            std::string bigram = words[i] + "_" + words[i + 1];
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
inline std::string trigramLookup(const Entry (&fixed_ngrams)[N],
                                 const std::vector<std::string>& words, Reorder reorder_helpers = nullptr, NounLookup nounLookup = nullptr, bool non_spaced = false) { 
                                    std::vector<std::string> mended;
                                    std::vector<int> ignore_flags(words.size(), 0);

    size_t i = 0;
    while (i < words.size()) {
        if (i + 2 < words.size()) {
            std::string trigram = words[i] + "_" + words[i + 1] + "_" + words[i + 2];
            
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





inline void to_lower(char *s) {
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] - 'A' + 'a';
        }
    }
}


inline std::vector<std::string> tokenize(const std::string &text) {
      std::vector<std::string> tokens;
      std::string current;
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
                      tokens.push_back(std::string(1, c));
              }
              ++i;
          } else {
              size_t len = 0;
              if ((c & 0xE0) == 0xC0) len = 2;
              else if ((c & 0xF0) == 0xE0) len = 3;
              else if ((c & 0xF8) == 0xF0) len = 4;
              else len = 1;

              std::string utf8char = text.substr(i, len);
              current += utf8char;
              i += len;
          }
      }

      if (!current.empty())
          tokens.push_back(current);
      return tokens;
  }

  inline std::vector<std::string> tokenize_cjk(const std::string &text) {
    std::vector<std::string> tokens;
    size_t i = 0;

    while (i < text.size()) {
        unsigned char c = text[i];

        if ((c & 0x80) == 0) {
            // ASCII alphanumeric word
            if (std::isalnum(c)) {
                std::string current;
                while (i < text.size() && (text[i] & 0x80) == 0 && std::isalnum(text[i])) {
                    current += text[i];
                    ++i;
                }
                tokens.push_back(current);
            } else {
                // ASCII punctuation or space
                if (!std::isspace(c))
                    tokens.push_back(std::string(1, c));
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


inline bool isPunctuation(const std::string &token) {
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


inline uint32_t next_utf8_char(const std::string& s, size_t& i) {
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
    void (*fp)(std::vector<Word>&, const Word&, const Word&, const char*);  
};


inline Action actions[] = {
    {"INVERT", &invert },
    {"REMOVE_FIRST", &remove_first},
    {"SANDWICH", &sandwich},
    {"REPLACE_FIRST", &replace_first}
};


inline std::vector<std::string> parser(const std::string& s) {
    std::vector<std::string> out;
    std::string current;

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
    -> void (*)(std::vector<Word>&, const Word&, const Word&, const char*)  
{
    int actionCount = sizeof(actions) / sizeof(actions[0]);

    for (int i = 0; i < actionCount; i++) {

        const char* p = query;
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
       const std::string& rule,
    const std::vector<Word>& sentence_arr,
    std::vector<Word>& reordered_arr,
    int i,
    const char* action_arg = nullptr 
) {
    std::vector<std::string> t = parser(rule);
    if (t.size() < 6) return false;
    if (t[0] != "IF") return false;

    std::string A_str = t[1];
    WordType A = typeFromString(A_str);

    if (t[2] != "THEN") return false;

    struct Matcher { bool isType; WordType type; std::string literal; };
    std::vector<Matcher> Bs;

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

    std::vector<std::string> actions;
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

    std::string word_curr = sentence_arr[i].word;

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



//maybe i'll move this somewhere else? it's too deterministic (even for a deterministic translator),
// maybe when i'm not feeling as lazy i'll feed the data from the modules. idk
// having this fixed here is completely out of place
// this is the ugliest code you will see ever, do not pay attention to it :(
// lowk might work tho, i'll tune the parameters later


// inline std::string detect_language(const char* sentence) {
//     std::string language = "Not Sure.";
//     std::vector<std::string> tokens = tokenize(std::string(sentence));
    
//     #if defined(DETECT_LANGUAGE) || defined(ALL) 
//     float en = 0.0f;
//     float pt = 0.0f;
//     float ja = 0.0f;
//     float es = 0.0f;
//     float fr = 0.0f;
//     float zh = 0.0f;
//     float sv = 0.0f;
//     float my = 0.0f;

//     for (size_t i = 0; i < tokens.size(); i++) {
//         const std::string& word = tokens[i];

        
//         if(word == "o" || word == "e") {
//             pt += 0.5f; 
//             en += 0.5f;
//         };

//         if(word.length() > 1 && std::string(word).substr(word.length() - 2) == "ll"){
//                    en += 1.0f;
//         }
//         if(word == "i") en += 0.5f;
//         if(word == "y"){ es += 0.5f; fr += 0.4f;}

//         if(word == "em" || word == "sou" || word == "eu"){ pt += 1.0f;}

//         if(word == "je" || word == "moi") {fr += 1.0f;}

//         if(word == "are" || word == "you") {en += 1.0f;}

//         if(word == "lo" || word == "la") {es += 1.0f;}

//         if(word == "tu") {fr += 0.5f; pt += 0.5f; es += 0.5f;}
//         if(word == "te") {fr += 0.5f; pt += 0.5f; es += 0.5f;}
//         if(word.back() == 'g') en += 7.0f;
//         auto first_char = [](const std::string &s) -> std::string {
//             if (s.empty()) return "";
//             size_t len = 1;
//             unsigned char c = static_cast<unsigned char>(s[0]);
//             if ((c & 0x80) == 0x00) len = 1;    
//             else if ((c & 0xE0) == 0xC0) len = 2;
//             else if ((c & 0xF0) == 0xE0) len = 3;  
//             else if ((c & 0xF8) == 0xF0) len = 4;  
//             return s.substr(0, len);
//         };

//         auto last_char = [](const std::string &s) -> std::string {
//             if (s.empty()) return "";
//             size_t i = s.size();
//             while (i > 0 && (static_cast<unsigned char>(s[i-1]) & 0xC0) == 0x80) --i;
//             return s.substr(i-1);
//         };

//         if (last_char(word) == std::string("é")) { 
//             fr += 0.7f;
//             pt += 0.4f;
//             es += 0.7f;
//         }
//         if (last_char(word) == std::string("ó")) { 
//             pt += 0.4f;
//             es += 0.8f;
//         }
//         if (first_char(word) == std::string("ç")) { 
//             pt -= 1.0f;
//             fr += 0.6f;
//         }
//         if (word.find("ñ") != std::string::npos){
//             es += 1.0f;
//         }
//         if (word.find("ç") != std::string::npos){
//             pt += 0.7f; fr += 0.6f;
//         }
//          if (word.find("k") != std::string::npos || word.find("y") != std::string::npos || word.find("w") != std::string::npos){
//             pt -= 1.0f;
//         }
//          if (word.find("å") != std::string::npos || word.find("ä") != std::string::npos || word.find("ö") != std::string::npos){
//             sv += 0.9f;
//             pt -= 1.0f;
//             en -= 1.0f;
//             es -= 1.0f;
//             fr -= 1.0f;
//         }
        
//         if (word.find("ão") != std::string::npos)
//            { pt += 0.9f;}
//         if (word.find("yo") != std::string::npos)
//             {es += 0.6f; en += 0.7f; fr -= 0.8f; ja += 0.5;
//             }
//         if (word.find("nh") != std::string::npos || word.find("lh") != std::string::npos){
//             pt += 1.0f;}

//         if (word.find("sh") != std::string::npos || word.find("wr") != std::string::npos || word.find("ys") != std::string::npos || word.find("hy") != std::string::npos)
//          {   en += 1.0f;}

//         if(word.find("wh") != std::string::npos)
//          {  en += 1.0f;}
        
//            if (word.find("ux") != std::string::npos || word.find("ée") != std::string::npos)
//           {  fr += 0.8f;}
//            if (word.find("uis") != std::string::npos)
//           {  fr += 1.0f;}
//             if (word.find("ois") != std::string::npos)
//           {  fr += 0.6f;pt += 0.6f;}
//          if (word.find("um") != std::string::npos || word.find("tu") != std::string::npos)
//           {  pt += 0.7f;}
//            if (word.find("mbre") != std::string::npos)
//           { 
//             en += 0.2f;
//              es += 0.7f;
//         }


//         if (word.find("ph") != std::string::npos) {
//             en += 0.4f;
//             fr += 0.2f;
//         }
//         if (word.find("eu") != std::string::npos) {
//             fr += 0.3f;
//             pt += 0.7f;
//         }

//         for (size_t j = 0; j < word.size(); ++j) {
//             uint32_t unicode_c = next_utf8_char(word, j);
//             does it have hiragana
//             if (unicode_c >= 0x3040 && unicode_c <= 0x309F) ja += 1.0;
//             does it have katakana
//             if (unicode_c >= 0x30A0 && unicode_c <= 0x30FF) ja += 1.0;
//             does it have ideograms?
//             if (unicode_c >= 0x4E00 && unicode_c <= 0x9FFF){
//                 ja += 0.5;
//                 zh += 0.5;
//             };
//                 if(unicode_c == 0x6211 || unicode_c == 0x4F60 ){
//                    ja -= 1.0;
//                    zh += 1.0;     
//                 }
//         }
//     }
    
//     float maxScore = en;
//     if (pt > maxScore) maxScore = pt;
//     if (ja > maxScore) maxScore = ja;
//     if (es > maxScore) maxScore = es;
//     if (fr > maxScore) maxScore = fr;
//     if (zh > maxScore) maxScore = zh;
//     if (sv > maxScore) maxScore = sv;
//     if (my > maxScore) maxScore = my;

//     if (maxScore == 0.0f) {
//         language = "unknown";
//     } else if (maxScore == en) {
//         language = "en";
//     } else if (maxScore == pt) {
//         language = "pt";
//     } else if (maxScore == ja) {
//         language = "ja";
//     } else if (maxScore == es) {
//         language = "es";
//     } else if (maxScore == fr) {
//         language = "fr";
//     }else if (maxScore == zh) {
//         language = "zh";
//     }else if (maxScore == sv) {
//         language = "sv";
//     }else if (maxScore == my) {
//         language = "my";
//     }
//     #endif
    
//     return language;
// }

#ifdef ALLOW_IMPORTS


inline bool default_multibyte = false;
inline unsigned int ngrams_length = 0;
static inline Entry default_fixed_ngrams[] = {
  {"", ""}
};

inline unsigned int nouns_length = 0;
inline Entry default_nouns[1000];

inline unsigned int verbs_length = 0;
inline Verb default_verbs[1000];

inline std::vector<std::string> temp_verb_endings;
inline std::string temp_required_ending;
inline std::string temp_affix;
inline uint8_t temp_type = 0;


// whenever parsing the binary, use push_back instead of [i] = value to add new entries.
VERB_ENDINGS(default_endings, {
   {{""}, NONE, INFINITIVE}
});

inline int conjugations_length = 0;

VERB_CONJUGATIONS(default_conjugations, 
{
   {INFINITIVE, SUFFIX, "", "る"}
}
);

inline unsigned int adjective_length = 0;
inline Entry default_adjectives[1000];

inline unsigned int pronoun_length = 0;
inline Entry default_pronouns[20];


inline unsigned int adverb_length = 0;
inline Entry default_adverbs[1000];

inline char buffer[250];

//default for binary modules


struct NormalizationRule {
     std::string from;
     std::string to;
     int type;
};
inline unsigned int target_flags_length = 0;
inline unsigned int normalizationRuleLength = 0;
inline NormalizationRule default_normalizationRules[50];

inline std::string default_normalize(
    std::string word,
    NormalizationRule* rules,
    unsigned int rule_count
) {
    std::string normalized_ = word;
     for (unsigned int i = 0; i < rule_count; ++i)
{
    NORMALIZE(
        rules[i].from,
        rules[i].type,
        rules[i].to
    );
}
    return normalized_;
}


static std::vector<Word> default_reorderHelpers(const std::vector<Word>& copy){
    std::vector<Word> sentence_arr = copy;
    std::vector<Word> reordered_arr;

    for (size_t i = 0; i < sentence_arr.size(); ++i) {
     INIT_REORDER()
      if (sentence_arr.at(i).type != -1){\
             reordered_arr.push_back(Word{ sentence_arr.at(i).word, sentence_arr.at(i).translation, sentence_arr.at(i).type});\
      }
    }
CLEANUP(reordered_arr)

std::vector<Word> final_arr;
for (size_t i = 0; i < reordered_arr.size(); ++i) {
    final_arr.push_back(reordered_arr[i]);
}
    return final_arr;
}


static Word default_verb_lookup(const std::string& word) {
    if (verbs_length == 0 || default_endings.empty() || default_conjugations.empty())
        return Word{word, word, -1};

    bool multibyte = default_multibyte;

    for (const auto& group : default_endings) {
        for (const auto& ending : group.endings) {
            // Check if word ends with this ending
            if (word.size() <= ending.size()) continue;
            
            bool ends_with = false;
            if (multibyte) {
                // Simple byte comparison works for UTF-8 suffixes
                if (word.compare(word.size() - ending.size(), ending.size(), ending) == 0)
                    ends_with = true;
            } else {
                if (word.compare(word.size() - ending.size(), ending.size(), ending) == 0)
                    ends_with = true;
            }
            if (!ends_with) continue;

            std::string root = word.substr(0, word.size() - ending.size());

            // Look up root in verbs dictionary
            Verb v = find_verb_in_array(default_verbs, verbs_length, root.c_str());
            if (v.translation && *v.translation) {
                std::string translation = v.translation;

                // Find matching conjugation
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
                        std::string result;
                        if (conj.type == PREFIX)
                            result = conj.affix + translation;
                        else // SUFFIX
                            result = translation + conj.affix;
                        return Word{word, result, VERB};
                    }
                }
            }
        }
    }
    return Word{word, word, -1};
}


static Word default_nounLookup(const std::string& word) {
    if (const char* result = lookup_test(default_nouns, nouns_length,word.c_str())) { 
        std::string translation = result; 
        int word_type = NOUN; 
        return { word, default_normalize(translation, default_normalizationRules,normalizationRuleLength), word_type }; 
    }else if(const char* result = lookup_test(default_adjectives, adjective_length,word.c_str())){
            std::string translation = result; 
            int word_type = ADJECTIVE; 
            return { word, default_normalize(translation, default_normalizationRules,normalizationRuleLength), word_type }; 
    }else if(const char* result = lookup_test(default_pronouns, pronoun_length,word.c_str())){
            std::string translation = result; 
            int word_type = ADJECTIVE; 
            return { word, default_normalize(translation, default_normalizationRules,normalizationRuleLength), word_type }; 
    }else if(const char* result = lookup_test(default_adverbs, adverb_length,word.c_str())){
            std::string translation = result; 
            int word_type = ADVERB; 
            return { word, default_normalize(translation, default_normalizationRules,normalizationRuleLength), word_type }; 
    }

    Word vw = default_verb_lookup(word);
    if (vw.type != -1) return vw;
    return Word{ word, default_normalize(word, default_normalizationRules,normalizationRuleLength), -1 };  

}

// mapping out how i'm gonna receive the binary file buffers to dinamically define the rules 
// it works!!
inline std::string load_from_bin(const uint8_t* file, size_t size)
{
    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(file);
    const uint8_t* end = ptr + size;
    int current_area = 0;
    const char* s = nullptr;
    const char* r = nullptr;
    std::string from = "";
    std::string to = "";
    default_endings.clear();
    default_conjugations.clear();
    conjugations_length = 0;
    nouns_length = 0;
    verbs_length = 0;
    adjective_length = 0;

    uint8_t word_length = 0;
    uint8_t ngram_size = 0x01;
    uint8_t type = 0x00;
    
    static uint8_t target_flags = 0;

    while (ptr < end)
    {
          uint8_t marker = *ptr++;
  

        switch (marker)
        {
            case 0xD0:
            current_area = 0;
            break;

        case 0xD1:
            current_area = 1;
            break;
        case 0xD2:
            current_area = 2;
            break;
        case 0xD3:
            current_area = 3;
            break;
        case 0xD4:
            current_area = 4;
            break;
        case 0xD5:
            current_area = 5;
            break;
case 0xF0:
{
    if (current_area == 0) // metadata
    {
        word_length = *ptr++;
        s = reinterpret_cast<const char*>(ptr);
        from = std::string(s);
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
            std::vector<std::string> endings;
            for (int i = 0; i < ending_count; i++) {
                s = reinterpret_cast<const char*>(ptr);
                std::string ending = std::string(s);
                endings.push_back(ending);
                ptr += strlen(s) + 1;
            }
            temp_verb_endings = endings;
        }
        else { // current_area == 4
            s = reinterpret_cast<const char*>(ptr);
            temp_required_ending = std::string(s);
            ptr += strlen(s) + 1;
            s = reinterpret_cast<const char*>(ptr);
            temp_affix = std::string(s);
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
        to = std::string(r);
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
        default_normalizationRules[normalizationRuleLength++] = 
            NormalizationRule{s, r, type};
    }
    else if (current_area == 3) {
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
        // Create VerbConjugation from temp data
        VerbConjugation conj;
        conj.form = type; // form flags (INFINITIVE, PAST, etc.)
        conj.type = temp_type; // SUFFIX/PREFIX
        conj.required_ending = temp_required_ending;
        conj.affix = temp_affix;
        
        default_conjugations.push_back(conj);
        conjugations_length++;
    }
       else if (current_area == 5) {
        // Create normalization rule
        if (s && r) {
            default_normalizationRules[normalizationRuleLength++] = 
                NormalizationRule{std::string(s), std::string(r), type};
        }
        // Reset pointers to avoid reuse
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
        to = std::string(r);
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
        to = std::string(r);
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
std::cout << "Nouns: " << nouns_length << "\n";
std::cout << "Adjectives: " << adjective_length << "\n";
std::cout << "Pronouns: " << pronoun_length << "\n";
std::cout << "Multi-word entries: " << ngrams_length << "\n";
std::cout << "Verbs: " << verbs_length << "\n";
std::cout << "Verb endings: " << default_endings.size() << "\n";
std::cout << "Verb conjugations: " << default_conjugations.size() << "\n";
std::cout << "Normalization rules: " << normalizationRuleLength << "\n";
std::cout << "Multibyte? " << (default_multibyte == 0 ? "No" : "Yes") << "\n";
return std::string("Loaded translator: " + from + " > " + to + "\n");
}
inline std::string translate_from_bin(const char* sentence,
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
  std::vector<std::string> arr; 
 if(default_multibyte == true) { 
    arr = tokenize_cjk(std::string(buffer));
    std::vector<std::string> verb_combined; 
    if (!(verbs_length < 1) && !default_endings.empty()) {
        DEFAULT_COMBINE_VERB_TOKENS(arr, verb_combined, default_verbs, default_endings, 3, 2); 
        std::vector<std::string> final_combined; 
        size_t i = 0; 
        while (i < verb_combined.size()) { 
            bool found = false; 
            for (size_t len = 4; len >= 1; --len) { 
                if (i + len > verb_combined.size()) continue; 
                std::string cand; 
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
    arr = tokenize(std::string(buffer)); 
} 

    if (arr.empty()) {
        return "";
    }

    std::string translated =
        trigramLookup(default_fixed_ngrams,
                      arr,
                      default_reorderHelpers,
                      default_nounLookup);

    return translated;
}

#endif


inline std::string translate(const char* sentence, const char* from, const char* to, int script = 2, bool auto_correct = false){
    // script will be passed to languages that can be written in more than one script
    // japanese (0 = kana, 1 = kana + kanji, 2 = romaji ), malay(0 = rumi, 1 = jawi) and mandarin chinese (0 = hanzi, 1 = pinyin) 
    //  are the ones i plan to implement

    std::string f(from), t(to);
    #if defined(PT_EN) || defined(ALL)
        if ((f == "pt" || f == "PT") && (t == "en" || t == "EN")) {
            return traduzir_en(sentence, auto_correct);
        }
    #endif

    #if defined(PT_MBL) || defined(ALL)
        if ((f == "pt" || f == "PT") && (t == "mbl" || t == "MBL")) {
            return pt_mbl(sentence);
        }
    #endif

     #if defined(JA_MBL) || defined(ALL)
        if ((f == "ja" || f == "JA") && (t == "mbl" || t == "MBL")) {
            return ja_mbl(sentence);
        }
    #endif
    
     #if defined(AR_TH) || defined(ALL)
        if ((f == "ar" || f == "AR") && (t == "th" || t == "TH")) {
            return ar_th(sentence);
        }
    #endif

      #if defined(PT_RU) || defined(ALL)
        if ((f == "pt" || f == "PT") && (t == "ru" || t == "RU")) {
            return pt_ru(sentence);
        }
    #endif
    #if defined(PT_JA) || defined(ALL)
        if ((f == "pt" || f == "PT") && (t == "ja" || t == "JA")) {
            return pt_ja(sentence);
        }
    #endif

    #if defined(AA_BB) || defined(ALL)
        if ((f == "aa" || f == "AA") && (t == "bb" || t == "BB")) {
            return aa_bb(sentence);
        }
    #endif


    #if defined(PT_ES) || defined(ALL)
        if ((f == "pt" || f == "PT") && (t == "es" || t == "ES")) {
            return pt_es(sentence);
        }
    #endif
    #if defined(PT_SV) || defined(ALL)
        if ((f == "pt" || f == "PT") && (t == "sv" || t == "SV")) {
            return traduzir_sv(sentence);
        }
    #endif
    #if defined(EN_JA) || defined(ALL)
        if ((f == "en" || f == "EN") && (t == "ja" || t == "JA")) {
            return translate_ja(sentence, script);
        }
    #endif
     #if defined(MA_ZH) || defined(ALL)
      if ((f == "ma") && (t == "zh")) {
          return ma_zh(sentence); 
      }
          #endif

  #if defined(EN_PT) || defined(ALL)
        if ((f == "en" || f == "EN") && (t == "pt" || t == "PT")) {
            return translate_pt(sentence);
        }
    #endif

#if defined(ZH_EN) || defined(ALL)
        if ((f == "zh" || f == "ZH") && (t == "en" || t == "EN")) {
            return translate_zh(sentence);
        }
#endif

      return "No translation module found :(";
}




extern Homonym homonyms[];
extern const size_t homonymCount;

// this can be global, since specific cases are defined on the structs and not on the function itself
inline std::string semantics(const std::vector<std::string>& sentence,
                             const std::vector<int>& word_types,
                             size_t index,
                             Homonym* homonyms,
                             size_t numHomonyms)
{
    if (index >= sentence.size()) return sentence[index];

    const std::string& w = sentence[index];
   for (size_t h = 0; h < numHomonyms; ++h) {
        Homonym& hom = homonyms[h];
        if (!hom.word) continue;
        if (std::string(hom.word) != w) continue;

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

            const std::string& nearby = sentence[nearbyIdx];

            size_t currentOutcome = 0;
            if (!hom.tokens) continue;

            for (size_t tidx = 0; tidx < hom.num_tokens && currentOutcome < hom.num_outcomes; ++tidx) {
                std::string token = hom.tokens[tidx];
     
                if (token == "$") { 
                    ++currentOutcome; 
                    continue; 
                }

                if (nearby == token) {
                    bool forbidden = false;

                    int prevIdx = nearbyIdx - 1; 
                    int prevType = (prevIdx >= 0) ? word_types[prevIdx] : -1;

                    if (prevType >= 0 && hom.forbidden_previous_type) {
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
        std::string bestWord = w;
        for (size_t j = 0; j < hom.num_outcomes; ++j) {
            if (hom.outcomes && j < hom.num_outcomes) {
                Outcome& o = hom.outcomes[j];
                if (o.score > bestScore) {
                    bestScore = o.score;
                    bestWord = o.word ? std::string(o.word) : w;
                }
            }
        }
        return bestWord;
    }

    return w;
}



/* ------- SHARED FUNCTIONS ----------------
|           some pairs have to use         |  
------------------------------------------*/

// PT


#if defined(PT_EN) || defined(PT_ES) || defined(ALL)

//is the word visibly not imported from a different language? 
// had to do this since stuff like 'website' is conflicting with verb ending match 'ite',
// but if we infer wether or not the word is an import, we can stop that
inline bool isPortuguese(const std::string& word){

  bool isIt = true;

  for(int i = 0; i < word.length(); ++i){
    char letter = word[i];
    // out of the way we know that there arent native portuguese words with w,y or k. so........  
        if(letter == 'w' || letter == 'y' || letter == 'k'){
             return false;
      }
      // there must be other syllables that could fit here to extract this info to but i'll do that another day :p

    }
    return isIt;
}

inline bool isDiminutive(const std::string& s, const char* suffix) {
    size_t n = 0;
    while (suffix[n] != '\0') n++; 
    if (s.size() < n) return false;
    for (size_t i = 0; i < n; i++) {
        if (s[s.size() - n + i] != suffix[i]) return false;
    }
    return true;
}


#endif



#endif
