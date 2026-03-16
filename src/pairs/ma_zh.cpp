#include "../lang_it.h"
#if defined(ALL)
#define MA_ZH // arbitrarily choose a name for the pair and define it, FROM_TO is enough
#endif

#ifdef MA_ZH // wrap the whole code in a definition check

#include <string>
#include <vector>

using namespace std;


DICT(fixed_ngrams, {
 {"", ""},
});

//create a basic dictionary mapping {word, translation}
DICT(nouns, {
 {"kucing", "猫"},
 {"anjing", "狗"},
 {"air", "水"},
 {"cendawan", "蘑菇"}
 }); 


 

static string normalize(string word) {
    string normalized_ = word;
    return normalized_;
}

static std::vector<Word> reorder_helpers(const std::vector<Word>& copy){
    std::vector<Word> sentence_arr = copy;
    vector<Word> reordered_arr;

    for (size_t i = 0; i < sentence_arr.size(); ++i) {
     INIT_REORDER()
     DEFAULT()
    }

    CLEANUP(reordered_arr);

vector<Word> final_arr;
for (size_t i = 0; i < reordered_arr.size(); ++i) {
    final_arr.push_back(reordered_arr[i]);
}
return final_arr;
}


static Word nounLookup(const string& word) {
    // add a simple lookup macro:
    //looking for an entry in the 'nouns' dict, if found assign NOUN type, and the word to look for (which is a token in a sentence we typed), since neither malay nor chinese uses grammatical gender, we pass null pointers.

    LOOKUP(nouns, NOUN, word, (Gender*)nullptr, (Gender*)nullptr);
    return { word, word, -1 };
}

MAIN(ma_zh, fixed_ngrams, reorder_helpers, nounLookup)
#endif