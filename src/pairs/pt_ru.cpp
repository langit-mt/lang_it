#include "../lang_it.h"
#if defined(ALL)
#define FROM "pt"
#define TO "ru"
#define PT_RU

#endif

#ifdef PT_RU

#include <string>
#include <vector>
#include <iostream>

using namespace std;


Info pt_ru_info = {
    SVO,
    {0},
    SVO, 
    {1}
};





CASE_DEF(ptru_case_to, SUFFIX,
{
  {ACCUSATIVE, FEMININE_GENDER, "а", "у"},   // собака → собаку
  {GENITIVE, FEMININE_GENDER, "а", "чья"},
  {GENITIVE, ON, "", ""}
    {ACCUSATIVE, FEMININE_GENDER, "я", "ю"},   // неделя → неделю
    {ACCUSATIVE, ON, "", ""},    // мужчина → мужчину handled separately maybe
    {ACCUSATIVE, NEUTRAL_GENDER, "о", "о"},    // окно → окно (same for accusative)
   {ACCUSATIVE, NEUTRAL_GENDER, "е", "е"}     // море → море
});

DICT(fixed_ngrams, {
 {"por_exemplo", "например"},
 
});

DICT(adj, {
 {"quiet", "тих"},
 {"vazi", "пуст"}
});

DICT(nouns, {
 {"olá", "привет"},
 {"cobra", "змея"},
 {"cachorr", "собака", FEMININE_GENDER},
 {"familia", "семья", FEMININE_GENDER},
 {"gat", "кот"},
 {"hoje", "сегодня"},
 {"amanhã", "завтр",FEMININE_GENDER},
 {"livro", "книга",0, FEMININE_GENDER},
 {"mundo", "мир"},
 {"noite", "ночь"},
 {"açucar", "сахар"},
 {"leite", "молоко"},
 {"criança", "ребенок"},
 {"grama", "трава", FEMININE_GENDER},
 {"tradutor", "переводчик",NEUTRAL_GENDER},
 {"manga", "манго"},
 {"juice", "сок"},
 {"ontem", "вчера"},
 {"camisa", "рубашка", FEMININE_GENDER}
});

DICT(pro, {
 {"eu", "я"},
 {"ele", "он"},
 {"ela", "она", 0, FEMININE_GENDER },
 {"você", "ты"},
 {"sou", ""}
});

DICT(obl_pro, {
 {"te", "тебя"},
 {"me" "себя"}
});



DICT(adv, {
 {"também", "тоже"},
 {"e", "и", 0, CONJUNCTIVE}
});

DICT(art, {
 {"o", "o"},
 {"a", "a"},
 {"as", "as"},
 {"os", "os"},
 {"um", "um"},
 {"uma", "uma"}
});

V_DICT(verbs, {
    {"sab", "зна"},
    {"v", "виде"},
    {"funcio", "работа"},
    {"l", "чита"}
});
#define PT_INF          INFINITIVE

// Portuguese past tense combinations
#define PT_PAST_1SG     (PAST | INDICATIVE | SIMPLE | ACTIVE | FIRST_PERSON | SINGULAR)
#define PT_PAST_2SG     (PAST | INDICATIVE | SIMPLE | ACTIVE | SECOND_PERSON | SINGULAR)
#define PT_PAST_3SG     (PAST | INDICATIVE | SIMPLE | ACTIVE | THIRD_PERSON | SINGULAR)
#define PT_PAST_1PL     (PAST | INDICATIVE | SIMPLE | ACTIVE | FIRST_PERSON | PLURAL_V)
#define PT_PAST_3PL     (PAST | INDICATIVE | SIMPLE | ACTIVE | THIRD_PERSON | PLURAL_V)

// Portuguese present tense combinations
#define PT_PRES_1SG     (PRESENT | INDICATIVE | SIMPLE | ACTIVE | FIRST_PERSON | SINGULAR)
#define PT_PRES_2SG     (PRESENT | INDICATIVE | SIMPLE | ACTIVE | SECOND_PERSON | SINGULAR)
#define PT_PRES_3SG     (PRESENT | INDICATIVE | SIMPLE | ACTIVE | THIRD_PERSON | SINGULAR)
#define PT_PRES_1PL     (PRESENT | INDICATIVE | SIMPLE | ACTIVE | FIRST_PERSON | PLURAL_V)
#define PT_PRES_2PL     (PRESENT | INDICATIVE | SIMPLE | ACTIVE | SECOND_PERSON | PLURAL_V)
#define PT_PRES_3PL     (PRESENT | INDICATIVE | SIMPLE | ACTIVE | THIRD_PERSON | PLURAL_V)

// Russian target combinations
#define RU_INF          INFINITIVE
#define RU_PAST_MASC    (PAST | INDICATIVE | SIMPLE | ACTIVE | SINGULAR | MASCULINE)
#define RU_PAST_FEM     (PAST | INDICATIVE | SIMPLE | ACTIVE | SINGULAR | FEMININE_V)
#define RU_PAST_NEUT    (PAST | INDICATIVE | SIMPLE | ACTIVE | SINGULAR | NEUTER)
#define RU_PAST_PL      (PAST | INDICATIVE | SIMPLE | ACTIVE | PLURAL_V)

#define RU_PRES_1SG     (PRESENT | INDICATIVE | SIMPLE | ACTIVE | FIRST_PERSON | SINGULAR)
#define RU_PRES_2SG     (PRESENT | INDICATIVE | SIMPLE | ACTIVE | SECOND_PERSON | SINGULAR)
#define RU_PRES_3SG     (PRESENT | INDICATIVE | SIMPLE | ACTIVE | THIRD_PERSON | SINGULAR)
#define RU_PRES_1PL     (PRESENT | INDICATIVE | SIMPLE | ACTIVE | FIRST_PERSON | PLURAL_V)
#define RU_PRES_2PL     (PRESENT | INDICATIVE | SIMPLE | ACTIVE | SECOND_PERSON | PLURAL_V)
#define RU_PRES_3PL     (PRESENT | INDICATIVE | SIMPLE | ACTIVE | THIRD_PERSON | PLURAL_V)


// Portuguese verb endings (source language)
VERB_ENDINGS(pt_ru_reg, {
   // Portuguese infinitive endings
   {{"ar", "er", "ir"}, PT_INF},
   
   // Portuguese past tense endings
   {{"ei"}, PT_PAST_1SG},        // falei
   {{"ou", "eu"}, PT_PAST_3SG},        // falou
   {{"aram"}, PT_PAST_3PL},      // falaram
   {{"aste"}, PT_PAST_2SG},      // falaste
   {{"amos"}, PT_PAST_1PL},      // falamos
   {{"i"}, PT_PAST_1SG},         // parti (for -ir verbs)
   
   // Portuguese present tense endings
   {{"o"}, PT_PRES_1SG},         // falo
   {{"as"}, PT_PRES_2SG},        // falas
   {{"a"}, PT_PRES_3SG},         // fala
   {{"amos"}, PT_PRES_1PL},      // falamos
   {{"ais"}, PT_PRES_2PL},       // falais
   {{"am"}, PT_PRES_3PL},        // falam
   
});

// Russian conjugation rules (target language)
VERB_CONJUGATIONS(pt_ru_def, 
{
   // Infinitive mapping
   {RU_INF, SUFFIX, "", "ть"},
   
   // Past tense mapping - Russian merges all persons, distinguishes gender
   {PT_PAST_1SG, SUFFIX, "", "л"},    // я говорил (masc)
   {PT_PAST_2SG, SUFFIX, "", "л"},    // ты говорил
   {PT_PAST_3SG, SUFFIX, "", "л"},    // он говорил
   {PT_PAST_1PL, SUFFIX, "", "ли"},   // мы говорили
   {PT_PAST_3PL, SUFFIX, "", "ли"},   // они говорили
   
   // Feminine past (requires the stem to end with "a" or have feminine marker)
   {PT_PAST_1SG, SUFFIX, "", "ла"},  // я говорила
   {PT_PAST_2SG, SUFFIX, "", "ла"},  // ты говорила
   {PT_PAST_3SG, SUFFIX, "", "ла"},  // она говорила
   
   // Present tense - 1st conjugation (е/ё endings)
   {PT_PRES_1SG, SUFFIX, "", "ю"},     // говорю
   {PT_PRES_2SG, SUFFIX, "", "ешь"},   // говоришь
   {PT_PRES_3SG, SUFFIX, "", "ет"},    // говорит
   {PT_PRES_1PL, SUFFIX, "", "ем"},    // говорим
   {PT_PRES_2PL, SUFFIX, "", "ете"},   // говорите
   {PT_PRES_3PL, SUFFIX, "", "ют"},    // говорят
   
   // 2nd conjugation pattern (for -ить verbs)
   {PT_PRES_2SG, SUFFIX, "и", "ишь"},  // любишь
   {PT_PRES_3SG, SUFFIX, "и", "ит"},   // любит
   {PT_PRES_1PL, SUFFIX, "и", "им"},   // любим
   {PT_PRES_2PL, SUFFIX, "и", "ите"},  // любите
   {PT_PRES_3PL, SUFFIX, "и", "ят"}    // любят
}
);
SUFFIX_RULES(suff, {
  {"mente", "о", ADVERB},
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
        { "манго", 0 },
        { "рукав", 0 }
    ),
    HOMONYM_FORBIDDEN(98, 99),
    "сок","$", "рубашка"
);

Homonym pt_ru_homonyms[] = {
    HOMONYM("manga", manga)
};


  GENDER_DEF(ptru_gender_from, SUFFIX,
        {
           {NONE, "o"},
           {FEMININE_GENDER, "a"}
        });

    GENDER_DEF(ptru_gender_to, SUFFIX,
    {
       {NONE, "ий"},
       {FEMININE_GENDER, "ая"},
       {NEUTRAL_GENDER, "ое"}
    });

     PLURAL_DEF(ptru_plural_from, SUFFIX,
        {
           {NONE, "s", ""}
        });

    PLURAL_DEF(ptru_plural_to, SUFFIX,
    {
       {NEUTRAL_GENDER, "о", "а"},
       {FEMININE_GENDER, "я", "и"},
       {FEMININE_GENDER, "ь","и"},
       {FEMININE_GENDER, "а", "ы"}
    });





static string normalize(string word) {
    string normalized_ = word;
    NORMALIZE("tec", REPLACE_START, "тех");
    NORMALIZE("b", REPLACE_ALL, "б");
    NORMALIZE("c", REPLACE_ALL, "к");
    NORMALIZE("d", REPLACE_ALL, "д");
    NORMALIZE("f", REPLACE_ALL, "ф");
    NORMALIZE("g", REPLACE_ALL, "г");
    NORMALIZE("h", REPLACE_ALL, "х");
    NORMALIZE("j", REPLACE_ALL, "ж");
    NORMALIZE("k", REPLACE_ALL, "к");
    NORMALIZE("l", REPLACE_ALL, "л");
    NORMALIZE("m", REPLACE_ALL, "м");
    NORMALIZE("n", REPLACE_ALL, "н");
    NORMALIZE("p", REPLACE_ALL, "п");
    NORMALIZE("r", REPLACE_ALL, "р");
    NORMALIZE("s", REPLACE_ALL, "с");
    NORMALIZE("t", REPLACE_ALL, "т");
    NORMALIZE("v", REPLACE_ALL, "в");
    NORMALIZE("w", REPLACE_ALL, "в");
    NORMALIZE("x", REPLACE_ALL, "кс");
    NORMALIZE("y", REPLACE_ALL, "и");
    NORMALIZE("z", REPLACE_ALL, "з");
    NORMALIZE("a", REPLACE_ALL, "а");
    NORMALIZE("e", REPLACE_ALL, "е");
    NORMALIZE("i", REPLACE_ALL, "и");
    NORMALIZE("o", REPLACE_ALL, "о");
    NORMALIZE("u", REPLACE_ALL, "у");
    return normalized_;
}

static std::vector<Word> reorder_helpers(const std::vector<Word>& copy){
    std::vector<Word> sentence_arr = copy;
    vector<Word> reordered_arr;

    for (size_t i = 0; i < sentence_arr.size(); ++i) {
     INIT_REORDER()
     RULE("IF ARTICLE THEN NOUN DO REMOVE_FIRST")
     
     RULE("IF OBLIQUE_PRONOUN THEN VERB DO INVERT")


     

     DEFAULT()

    }

    CLEANUP(reordered_arr);
    
    HANDLE_CASE(&pt_ru_info, NO_CASE,&ptru_case_to)

    
vector<Word> final_arr;
for (size_t i = 0; i < reordered_arr.size(); ++i) {
    final_arr.push_back(reordered_arr[i]);
}


final_arr = MEDIATE_HOMONYMS(final_arr, {"manga", "banco"}, pt_ru_homonyms);


return final_arr;
}


static Word nounLookup(const string& word) {
 
  

    LOOKUP(nouns, NOUN, word, &ptru_gender_from, NO_GENDER, &ptru_plural_from, &ptru_plural_to);
    
    LOOKUP(adj, ADJECTIVE, word, &ptru_gender_from,&ptru_gender_to, &ptru_plural_from, &ptru_plural_from);

    LOOKUP(pro, PRONOUN, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    LOOKUP(obl_pro, OBLIQUE_PRONOUN, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    LOOKUP(adv, ADJECTIVE, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    LOOKUP(art, ARTICLE, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    
    SUFFIX_LOOKUP(suff, word, adj);

    VERB_LOOKUP(verbs, word, pt_ru_reg, pt_ru_def, false);


    return { word, word, -1 };
}

MAIN(pt_ru, fixed_ngrams, reorder_helpers, nounLookup, false,false, pt_ru_reg)

#endif