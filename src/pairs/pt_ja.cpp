#include "../lang_it.h"
#if defined(ALL)
#define FROM "pt"
#define TO "ja"
#define PT_JA

#endif

#ifdef PT_JA

#include <string>
#include <vector>

using namespace std;

#define I_ADJECTIVE FREE_BIT_1
#define NA_ADJECTIVE FREE_BIT_2

Info pt_ja_info = {
    SVO,
    SOV, 

    { 
        {
        MIDDLE_WORD, // how does it act?
        2, // what word does it act upon, 0 = A or 1 = B or 2 = none?
        POSSESSED_FIRST, // 0 == B * A OR  1 = A * B ?
        0, //NO flags
        "do" 
        },
        {
        MIDDLE_WORD, // how does it act?
        2, // what word does it act upon, 0 = A or 1 = B or 2 = none?
        POSSESSED_FIRST, // 0 == B * A OR  1 = A * B ?
        FEMININE_GENDER, //NO flags
        "da" 
        },
        {
        MIDDLE_WORD, // how does it act?
        2, // what word does it act upon, 0 = A or 1 = B or 2 = none?
        POSSESSED_FIRST, // 0 == B * A OR  1 = A * B ?
        0, 
        "de" 
        }
    },
    { 
       {
        MIDDLE_WORD, // how does it act?
        2, // what word does it act upon, 0 = A or 1 = B or 2 = none?
        OWNER_FIRST, // 0 == B * A OR  1 = A * B ?
        0, //NO flags
        "の" 
        }
    },
    NOUN_FIRST,
    ADJECTIVE_FIRST,
     2,
     1,
     
    {0},
    {1}
};





CASE_DEF(ptja_case_to, NEXT_WORD,
{
    {ACCUSATIVE, 0, "", "を"},   
 
});

DICT(fixed_ngrams, {
 {"por_exemplo", "例えば"},
 
});

DICT(adj, {
 {"quiet", "静か", 0, NA_ADJECTIVE},
 {"vazi", "空っぽ", 0,NA_ADJECTIVE},
 {"grande", "大き", 0, NA_ADJECTIVE},
 {"azul", "青", 0, I_ADJECTIVE},
 {"normal", "普通", 0, NA_ADJECTIVE}
});

DICT(nouns, {
 {"olá", "こんにちは"},
 {"cobra", "蛇"},
 {"cachorr", "犬", 0, ANIMATE},
 {"gat", "猫", 0, ANIMATE},
 {"laranja", "オレンジ"},
 {"animal", "動物", 0, ANIMATE},
 {"pessoa", "人", 0, ANIMATE},
 {"familia", "家族"},
 {"ontem", "昨日"},
 {"manga", "マンゴ"},
 {"hoje", "今日"},
 {"aniversário", "誕生日"},
 {"é", "は"},
 {"sou", "は"},
 {"estou", "は"},
 {"está", "は"},
 {"são", "は"},
 {"camisa", "シャツ"},
 {"mundo", "世界"}
});

DICT(pro, {
 {"eu", "私"},
 {"ela", "彼女"},
 {"você", "あなた"}
});

DICT(obl_pro, {
 {"te", "あなたを"},
 {"me" "私を"}
});

DICT(poss_pro, {
 {"meu", "私の"},
 {"seu" "あなたの"}
});


DICT(adv, {
 {"também", "も"},
 {"e", "と"},
 {"se", "なら"}
});

DICT(art, {
 {"o", "o"},
 {"a", "a"},
 {"as", "as"},
 {"os", "os"}
});

V_DICT(verbs, {
    {"com", "食べ"},
    {"sab", "知"},
    {"v", "見"},
    {"funcio", "効"},
    {"corr", "走"}
});

VERB_ENDINGS(pt_ja_reg, {
   {{"ou", "ei", "aram", "eu", "i", "imos", "iu", "iram"}, SUFFIX, PAST},
   {{"ar", "er", "ir"}, SUFFIX,INFINITIVE},
   {{"jo", "no", "na", "ne"}, SUFFIX,PRESENT},
   {{"endo", "ando", "indo"}, SUFFIX,CONTINUOUS}
});

VERB_CONJUGATIONS(pt_ja_def, 
{
   {INFINITIVE, SUFFIX, "", "る"},
   {PAST, SUFFIX, "","た"},
   {PRESENT, SUFFIX, "","っている"},
   {CONTINUOUS, SUFFIX, "","っている"}
}
);

SUFFIX_RULES(suff, {
  {"mente", "に", ADVERB},
   {"ção", "ция", NOUN},
   {"ções", "ции", NOUN},
   {"dade", "ность", NOUN},
   {"ismo", "изм", NOUN},
   {"ista", "ист", NOUN},
   {"al", "альный", ADJECTIVE},
   {"ico", "ический", ADJECTIVE},
   {"logia", "логия", NOUN}
});


HOMONYM_DEF(
    manga,
    HOMONYM_OUTCOMES(
        { "マンゴ", NOUN },
        { "袖", NOUN }
    ),
    HOMONYM_FORBIDDEN(98, 99),
    "食べる", "果物","$", "シャツ"
);

Homonym pt_ja_homonyms[] = {
    HOMONYM("manga", manga)
};


  GENDER_DEF(ptja_gender_from, SUFFIX,
        {
           {NONE, "o"},
           {FEMININE_GENDER, "a"}
        });


MORPH_DEF(ptja_morph_from,
    {
        {0, "is", "l", SUFFIX, PLURAL_MORPH},  // ends with "is" → remove "is", add "l" (animais → animal)
        {0, "es", "s", SUFFIX, PLURAL_MORPH},  // ends with "es" → remove "es", add "s" (leões → leão)
        {0, "os", "", SUFFIX, PLURAL_MORPH},
        {0, "as", "", SUFFIX, PLURAL_MORPH},
        {0, "s", "", SUFFIX, PLURAL_MORPH},      // ends with "s" → remove "s", add nothing (gatos → gato)
        {0, "ão", "", SUFFIX, AUGMENTATIVE_MORPH},
        {0, "inho", "", SUFFIX, DIMINUTIVE_MORPH},
        {0, "o", "", PREV_WORD, DEFINITE_MORPH},
        {0, "ador", "o", SUFFIX, AGENT_MORPH}
    });
    MORPH_DEF(ptja_morph_to,
    {
       {ANIMATE, "", "たち", NEXT_WORD, PLURAL_MORPH},
       {0, "", "小", PREV_WORD, DIMINUTIVE_MORPH},
       {0, "", "大", PREV_WORD, AUGMENTATIVE_MORPH},
       {0, "", "", NONE, DEFINITE_MORPH}
    });





static string normalize(string word) {
    string normalized_ = word;
    NORMALIZE("ode", REPLACE_ALL,"オード");
    return normalized_;
}

static std::vector<Word> reorder_helpers(const std::vector<Word>& copy){

    std::vector<Word> sentence_arr = copy;
    vector<Word> reordered_arr;

    for (size_t i = 0; i < sentence_arr.size(); ++i) {
     INIT_REORDER()

          RULE("IF ARTICLE THEN NOUN DO REMOVE_FIRST")
          RULE("IF VERB THEN ADVERB DO INVERT")
          
     DEFAULT()

    }
 
    CLEANUP(reordered_arr);
    
    HANDLE_POSSESSION(&pt_ja_info, reordered_arr);

    
    HANDLE_CASE(&pt_ja_info, NO_CASE, &ptja_case_to)

    
    
vector<Word> final_arr;
for (size_t i = 0; i < reordered_arr.size(); ++i) {
    final_arr.push_back(reordered_arr[i]);
}
final_arr = INSERT_PARTICLES(final_arr, {
    {ADJECTIVE, NOUN, "い", (final_arr.size() >= 2), 
        I_ADJECTIVE, CHECK_FIRST_WORD, nullptr, -1},  
    {ADJECTIVE, NOUN, "な", (final_arr.size() >= 2), 
        NA_ADJECTIVE, CHECK_FIRST_WORD, nullptr, -1} ,
    {NOUN, ADVERB, "は", (final_arr.size() >= 2), 
        0, CHECK_FIRST_WORD, nullptr, -1}  
    });

final_arr = MEDIATE_HOMONYMS(final_arr, {"manga", "banco"}, pt_ja_homonyms);


return final_arr;
}


static Word nounLookup(const string& word) {
 
  

    LOOKUP(nouns, NOUN, word, &ptja_gender_from, NO_GENDER, &ptja_morph_from, &ptja_morph_to);
    
    LOOKUP(adj, ADJECTIVE, word, &ptja_gender_from,NO_GENDER, &ptja_morph_from, &ptja_morph_to);

    LOOKUP(pro, PRONOUN, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);
    LOOKUP(poss_pro, POSSESSIVE_PRONOUN, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    LOOKUP(obl_pro, OBLIQUE_PRONOUN, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    LOOKUP(adv, ADVERB, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    LOOKUP(art, ARTICLE, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    
    SUFFIX_LOOKUP(suff, word, adj);

    VERB_LOOKUP(verbs, word, pt_ja_reg, pt_ja_def,&ptja_morph_from, &ptja_morph_to, false);


    return { word, word, -1 };
}

MAIN(pt_ja, fixed_ngrams, reorder_helpers, nounLookup, 
     false,           // MULTIBYTE = false
     true,            // NON_SPACED = false
     pt_ja_reg,   // VERB_ENDINGS (can be empty)
     (true)           // DICT_CHECK (won't be used, but needs to be valid)
)
#endif