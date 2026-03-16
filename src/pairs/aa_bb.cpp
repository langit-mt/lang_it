/** esse é um experimento usando o par maxakali -> português */
/** this is an experiment using the maxakali -> portuguese pair */

#include "../lang_it.h"
#if defined(ALL)
#define AA_BB
#endif

#ifdef AA_BB

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;


DICT(fixed_ngrams, {
 {"como_sempre", "ehe"},
 
});

Entry nouns[] = {
  {"te", "you"},
  {"me", "me"},
};
unsigned int arr_length = 0;




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
     DEFAULT()
    }

    CLEANUP(reordered_arr)

vector<Word> final_arr;
for (size_t i = 0; i < reordered_arr.size(); ++i) {
    final_arr.push_back(reordered_arr[i]);
}
    return final_arr;
}


static Word nounLookup(const string& word) {
    LOOKUP(nouns, NOUN, word);
    return { word, word, -1 };
}

std::string aa_bb(const char* sentence) {
    char buffer[250];
int number_of_words = 100;
const char s[] = { 0x6B, 0x69, 0x64, 0 }; // kid
const char r[] = { 0x6B, 0x61, 0x6B, 0x78, 0x6F, 0x70, 0}; // kakxop

    if (arr_length < number_of_words) {
      nouns[arr_length++] = Entry{s, r, 0};
    } else {
    }


    strncpy(buffer, sentence, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';
    to_lower(buffer);
    std::vector<string> arr = tokenize(std::string(buffer));
    std::string translated = trigramLookup(fixed_ngrams, arr, reorder_helpers, nounLookup);
    return translated;
}
#endif