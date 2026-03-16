/** esse é um experimento usando o par árabe -> tailandês */
/** this is an experiment using the arabic -> thai pair */

#include "../lang_it.h"
#if defined(ALL)
#define AR_TH
#endif

#ifdef AR_TH

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

Info AR_TH_info = {
    VSO,  // Arabic is VSO
    {0},
    SVO,  // Thai is SVO
    {0}
};

// Fixed ngrams - Arabic input (no underscores needed in source)
DICT(fixed_ngrams, {
 {"السلام عليكم", "สวัสดี"},        // peace be upon you -> hello
 {"شكرا", "ขอบคุณ"},                 // thanks
 {"مع السلامة", "ลาก่อน"},            // goodbye
 {"كيف حالك", "สบายดีไหม"},           // how are you
 {"بسم الله", "ในนามของอัลลอฮ์"}       // in the name of Allah
});

// Adjectives
DICT(adj, {
 {"كبير", "ใหญ่"},                    // big
 {"صغير", "เล็ก"},                     // small
 {"جميل", "สวย"},                      // beautiful
 {"قبيح", "น่าเกลียด"},                 // ugly
 {"حار", "ร้อน"},                       // hot
 {"بارد", "เย็น"},                      // cold
 {"طويل", "สูง"},                       // tall
 {"قصير", "เตี้ย"}                       // short
});

// Nouns
DICT(nouns, {
 {"كتاب", "หนังสือ"},                   // book
 {"بيت", "บ้าน"},                        // house
 {"سيارة", "รถ"},                        // car
 {"ماء", "น้ำ"},                         // water
 {"خبز", "ขนมปัง"},                      // bread
 {"رجل", "ผู้ชาย"},                      // man
 {"امرأة", "ผู้หญิง"},                   // woman
 {"طفل", "เด็ก"},                        // child
 {"مدينة", "เมือง"},                     // city
 {"مدرسة", "โรงเรียน"}                   // school
});

// Pronouns
DICT(pro, {
 {"أنا", "ฉัน"},                         // I
 {"أنت", "คุณ"},                         // you (masculine)
 {"أنت", "เธอ"},                          // you (feminine) 
 {"هو", "เขา"},                           // he
 {"هي", "หล่อน"},                         // she
 {"نحن", "เรา"},                          // we
 {"أنتم", "พวกคุณ"},                      // you all
 {"هم", "พวกเขา"}                         // they
});

// Adverbs
DICT(adv, {
 {"جدا", "มาก"},                         // very
 {"أيضا", "ด้วย"},                        // also
 {"الآن", "ตอนนี้"},                       // now
 {"هنا", "ที่นี่"},                        // here
 {"هناك", "ที่นั่น"},                      // there
 {"دائما", "เสมอ"},                       // always
 {"أبدا", "ไม่เคย"}                        // never
});

// Verb stems
V_DICT(verbs, {
    {"كتب", "เขียน"},                      // write
    {"قرأ", "อ่าน"},                        // read
    {"أكل", "กิน"},                          // eat
    {"شرب", "ดื่ม"},                         // drink
    {"ذهب", "ไป"},                          // go
    {"جاء", "มา"},                          // come
    {"تكلم", "พูด"},                         // speak
    {"عمل", "ทำงาน"},                        // work
    {"درس", "เรียน"},                        // study
    {"لعب", "เล่น"},                         // play
    {"نام", "นอน"},                          // sleep
    {"فتح", "เปิด"},                          // open
    {"أغلق", "ปิด"}                          // close
});

// Arabic verb endings
VERB_ENDINGS(arabic_verb_endings, {
   {{"ت", "ت", "ت", "نا", "تم", "وا"}, PAST},      // past suffixes
   {{"أ", "ت", "ي", "ن", "ون", "ن"}, PRESENT}      // present prefixes/suffixes
});

// Thai doesn't have verb conjugations, but we need empty structure
VERB_ENDINGS(thai_dummy_endings, {});

VERB_CONJUGATIONS(arabic_verb_conjugations, {
   {NONE, NONE, "", ""}
});

// Homonyms
HOMONYM_DEF(
    عين,
    HOMONYM_OUTCOMES(
        { "ตา", 0 },        // eye
        { "น้ำพุ", 0 }       // spring/well
    ),
    HOMONYM_FORBIDDEN(98, 99),
    "เห็น", "น้ำ", "แหล่ง"
);

HOMONYM_DEF(
    يد,
    HOMONYM_OUTCOMES(
        { "มือ", 0 },        // hand
        { "แขน", 0 }         // arm
    ),
    HOMONYM_FORBIDDEN(98, 99),
    "จับ", "ถือ", "เขียน"
);

Homonym AR_TH_homonyms[] = {
    HOMONYM("عين", عين),
    HOMONYM("يد", يد)
};

static string normalize(string word) {
    string normalized_ = word;
    return normalized_;
}

static std::vector<Word> reorder_helpers(const std::vector<Word>& copy){
    std::vector<Word> sentence_arr = copy;

    std::vector<Word> combined;
    COMBINE_VERB_STEMS(sentence_arr, combined, verbs, arabic_verb_endings);
    sentence_arr = combined;

    vector<Word> reordered_arr;

    for (size_t i = 0; i < sentence_arr.size(); ++i) {
        INIT_REORDER()
        DEFAULT()
    }

    CLEANUP(reordered_arr);

    HANDLE_CASE(&AR_TH_info, NO_CASE, NO_CASE)

    vector<Word> final_arr;
    for (size_t i = 0; i < reordered_arr.size(); ++i) {
        final_arr.push_back(reordered_arr[i]);
    }

    // Thai particles (no spaces between words)
    final_arr = INSERT_PARTICLES(final_arr, {
        {NOUN, NOUN, "และ", (final_arr.size() > 2)},     // "and" between nouns
        {PRONOUN, NOUN, "ของ", (final_arr.size() > 2)}   // "of" for possession
    });

    final_arr = MEDIATE_HOMONYMS(final_arr, {"عين", "يد"}, AR_TH_homonyms);

    return final_arr;
}

static Word nounLookup(const string& word) {

    LOOKUP(nouns, NOUN, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);
    
    LOOKUP(adj, ADJECTIVE, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    LOOKUP(pro, PRONOUN, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    LOOKUP(adv, ADJECTIVE, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    VERB_LOOKUP(verbs, word, arabic_verb_endings, arabic_verb_conjugations, true);

    return { word, word, -1 };
}

// MAIN with DICT_CHECK combining Arabic source tokens that match any dictionary
MAIN(ar_th, fixed_ngrams, reorder_helpers, nounLookup, true, false, arabic_verb_endings,
     (lookup(nouns, cand.c_str()) || lookup(adj, cand.c_str()) || 
      lookup(pro, cand.c_str()) || lookup(adv, cand.c_str()) ||
      (verb_lookup(verbs, cand.c_str()).translation != nullptr)) 
)
#endif