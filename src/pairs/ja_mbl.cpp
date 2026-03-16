/** esse é um experimento usando o par maxakali -> português */
/** this is an experiment using the maxakali -> portuguese pair */

#include "../lang_it.h"
#if defined(ALL)
#define JA_MBL
#endif

#ifdef JA_MBL

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

Info JA_MBL_info = {
    SVO, // japanese is svo
    SVO, 
    {0}
    {2, 3}
};
#define I_ADJECTIVE FREE_BIT_1
#define NA_ADJECTIVE FREE_BIT_2



DICT(fixed_ngrams, {
 {"今_日", "hõnhã"},
 {"明_日", "hãptup putut"},
 {"世_界", "hãhãm"},
 {"佐_藤", "axok"},
 {"子_供", "kakxop"},
 {"で_も", "pa", 0, CONTRASTIVE},
 {"ミ_ル_ク", "xokhep"}
});

DICT(adj, {
 {"空", "hãmhok"}             
});

DICT(nouns, {
 {"蛇", "kãyã"},
 {"夜", "ãmũy"},
 {"mandioca", "kot"},
 {"segunda", "pa"},
 {"肉", "xogyĩn"},
 {"犬", "kokex"}
});

DICT(pro, {
 {"私", "ãte"},
 SYNONYMS("ha", "彼", "彼女", "これ")
});


DICT(adv, {
 {"も", "kama"},
 {"いつ", "hã"}
});

V_DICT(verbs, {
    {"押", "kaxix"},
    {"手伝", "ãyonat"},
    {"冷や", "ãxi"},
    {"飲", "xo'op"},
    {"食べ", "mãhã"},
    {"d", "hõm"},
    {"quer", "putup"},
    {"cheg", "xupep"},
    {"plant", "xok"},
    {"焼", "kutet"}
});

VERB_ENDINGS(jambl_verbs, {
   {{"た", "った", "しました", "みました", "いました", "ました", "きました", "いた"}, PAST},
   {{"す", "る", "う", "む", "く", "します", "います", "きます"}, INFINITIVE},
   {{"している", "しています", "na", "ne", "o"}, PRESENT},
   {{"endo", "ando", "indo"}, CONTINUOUS}
});

VERB_CONJUGATIONS(jambl_verbs_, 
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



Homonym JA_MBL_homonyms[] = {
    HOMONYM("manga", manga)
};

static string normalize(string word) {
    string normalized_ = word;
    NORMALIZE("rr", REPLACE_ALL, "h");
    return normalized_;
}

static std::vector<Word> reorder_helpers(const std::vector<Word>& copy){
    std::vector<Word> sentence_arr = copy;

     std::vector<Word> combined;
    COMBINE_VERB_STEMS(sentence_arr, combined, verbs, jambl_verbs);
    sentence_arr = combined;

    vector<Word> reordered_arr;

    for (size_t i = 0; i < sentence_arr.size(); ++i) {
     INIT_REORDER()


     DEFAULT()
    }

    CLEANUP(reordered_arr);


 HANDLE_CASE(&JA_MBL_info, NO_CASE, NO_CASE)

vector<Word> final_arr;
for (size_t i = 0; i < reordered_arr.size(); ++i) {
    final_arr.push_back(reordered_arr[i]);
}

final_arr = INSERT_PARTICLES(final_arr, {
    {NOUN, NOUN, "te", (final_arr.size() > 2)},
    {PRONOUN, NOUN, "te", (final_arr.size() > 2)}
});

final_arr = MEDIATE_HOMONYMS(final_arr, {"manga", "banco"}, JA_MBL_homonyms);

return final_arr;
}


static Word nounLookup(const string& word) {

    LOOKUP(nouns, NOUN, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);
    
    LOOKUP(adj, ADJECTIVE, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    LOOKUP(pro, PRONOUN, word, NO_GENDER,NO_GENDER, NO_PLURAL, NO_PLURAL);

    LOOKUP(adv, ADJECTIVE, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);


    VERB_LOOKUP(verbs, word, jambl_verbs, jambl_verbs_, true);

    return { word, word, -1 };
}

MAIN(ja_mbl, fixed_ngrams, reorder_helpers, nounLookup, true, false, jambl_verbs,
     (lookup(nouns, cand.c_str()) || lookup(adj, cand.c_str()) || lookup(pro, cand.c_str()) || lookup(adv, cand.c_str()))
)
#endif