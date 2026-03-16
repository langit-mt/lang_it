/** esse é um experimento usando o par maxakali -> português */
/** this is an experiment using the maxakali -> portuguese pair */

#include "../lang_it.h"
#if defined(ALL)
#define PT_MBL
#endif

#ifdef PT_MBL

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

Info pt_mbl_info = {
    SVO,
    {0},
    SVO, 
    {0}
};


DICT(fixed_ngrams, {
 {"como_sempre", "ehe"},
 
});

DICT(adj, {
 {"vazio", "hãmhok"},                          // SYNONYMS(traduz_para, um, dois, tres)  
 SYNONYMS("yĩxux", "azul", "amarelo", "verde") // nos dicionarios maxakali as palavras azul, amarelo, verde
                                                // sao todas traduzidas como yĩxux
});

DICT(nouns, {
 {"cobra", "kãyã"},
 {"hoje", "hõnhã"},
 {"amanhã", "hãptup putut"},
 {"mundo", "hãhãm"},
 {"noite", "ãmũy"},
 {"açucar", "axok"},
 {"leite", "xokhep"},
 {"criança", "kakxop"},
 {"mandioca", "kot"},
 {"segunda", "pa"},
 {"carne", "xogyĩn"},
 {"cachorr", "kokex"}
});

DICT(pro, {
 {"eu", "ãte"},
 SYNONYMS("ha", "ele", "ela", "isso")
});


DICT(adv, {
 {"também", "kama"},
 {"mas", "pa", 0, CONTRASTIVE},
 {"quando", "hã"}
});

DICT(art, {
 {"o", "o"},
 {"a", "a"},
 {"os", "os"},
 {"as", "as"}
});

V_DICT(verbs, {
    {"empurr", "kaxix"},
    {"ajud", "ãyonat"},
    {"esfri", "ãxi"},
    {"beb", "xo'op"},
    {"com", "mãhã"},
    {"d", "hõm"},
    {"quer", "putup"},
    {"cheg", "xupep"},
    {"plant", "xok"},
    {"ass", "kutet"}
});

VERB_ENDINGS(reg, {
   {{"ou", "ei", "aram", "eu", "i", "imos", "iu"}, PAST},
   {{"ar", "er", "ir", ""}, INFINITIVE},
   {{"jo", "no", "na", "ne", "o"}, PRESENT},
   {{"endo", "ando", "indo"}, CONTINUOUS}
});

VERB_CONJUGATIONS(def, 
{
   {NONE, NONE, "", ""},
}
);

HOMONYM_DEF(
    manga,
    HOMONYM_OUTCOMES(
        { "mango", 0 },
        { "sleeve", 0 }
    ),
    HOMONYM_FORBIDDEN(98, 99),
    "eat", "taste", "pick", "juice", "sweet",
    "candy", "dessert", "flavor", "taste",
    "ripe", "$", "shirt", "sew", "ripped",
    "rip", "button", "tight", "loose"
);


  GENDER_DEF(ptmbl_gender_from, SUFFIX,
        {
           {NONE, "o"},
           {FEMININE_GENDER, "a"}
        });

Homonym pt_mbl_homonyms[] = {
    HOMONYM("manga", manga)
};

static string normalize(string word) {
    string normalized_ = word;
    NORMALIZE("rr", REPLACE_ALL, "h");
    return normalized_;
}

static std::vector<Word> reorder_helpers(const std::vector<Word>& copy){
    std::vector<Word> sentence_arr = copy;
    vector<Word> reordered_arr;

    for (size_t i = 0; i < sentence_arr.size(); ++i) {
     INIT_REORDER()
     // maxacali não possui artigos, logo se um artigo é seguido por um substantivo, removemos ele
     // maxacali does not use articles, so if we have a noun following an article, we remove it
     RULE("IF ARTICLE THEN NOUN DO REMOVE_FIRST")

     // a ordem do português é SVO, já a do maxakalí na maioria das vezes é SOV
     // logo se temos um verbo e depois um substantivo em portugues, invertemos a ordem S[VO] -> S[OV]
     RULE("IF VERB THEN NOUN DO INVERT")

     DEFAULT()
    }

    CLEANUP(reordered_arr);


 HANDLE_CASE(&pt_mbl_info, NO_CASE, NO_CASE)

vector<Word> final_arr;
for (size_t i = 0; i < reordered_arr.size(); ++i) {
    final_arr.push_back(reordered_arr[i]);
}

final_arr = INSERT_PARTICLES(final_arr, {
    {NOUN, NOUN, "te", (final_arr.size() > 2)},
    {PRONOUN, NOUN, "te", (final_arr.size() > 2)}
});

final_arr = MEDIATE_HOMONYMS(final_arr, {"manga", "banco"}, pt_mbl_homonyms);

return final_arr;
}


static Word nounLookup(const string& word) {

    LOOKUP(nouns, NOUN, word, &ptmbl_gender_from, NO_GENDER, NO_PLURAL, NO_PLURAL);
    
    LOOKUP(adj, ADJECTIVE, word, &ptmbl_gender_from, NO_GENDER, NO_PLURAL, NO_PLURAL);

    LOOKUP(pro, PRONOUN, word, NO_GENDER,NO_GENDER, NO_PLURAL, NO_PLURAL);

    LOOKUP(adv, ADJECTIVE, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    LOOKUP(art, ARTICLE, word, NO_GENDER, NO_GENDER,  NO_PLURAL, NO_PLURAL);

    VERB_LOOKUP(verbs, word, reg, def, false);

    return { word, word, -1 };
}

MAIN(pt_mbl, fixed_ngrams, reorder_helpers, nounLookup, 
     false,           // MULTIBYTE = false
     false,            // NON_SPACED = false
     reg,   // VERB_ENDINGS (can be empty)
     (true)           // DICT_CHECK (won't be used, but needs to be valid)
)
#endif