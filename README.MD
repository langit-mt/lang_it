# lang_it

Langit means sky in some austronesian languages, that's it for the name choice.

lang_it is a rule-based, portable and lightweight universal translation engine written using only C++, with support for:

- Multi word entries
- Verb conjugation
- Morphology (Gender, number, case, diminutive, etc)
- Homonym disambiguation
- Simple auto-correct
- Multi-script languages (Latin, Hanzi, Kanji and Kana, Arabic-based, Cyrilic, Hangul)

## Technical Info

- ~59kb compiled footprint.
- C++ STL only, no external dependencies, runs anywhere as long as there's std::vector and std::string support.
- Can be compiled with g++, Clang or MSVC.
- Validated JS (WASM), Ruby (Rice), Python (pybind11) and Java (JNI) bindings. 

## Why?

In the age of ML based translation, why care about deterministic rule-based translation programs? 
For me personally i see the values in two aspects:

- Endangered languages with little validated corpora available for ML model training, but reasonable research on the grammar and vocabulary can be found.
- Linguistics analysis: lang_it can also produce tagged translations that can be used to generate statistics and reports on the language is translation into by seeing how does sentence in language A turns into sentence in language B.
- Portability: lang_it is super small compared to modern translation tools, and with enough module tuning, you can get very accurate translations, even with MCUs. You ever wanted your fridge to translate offline? now it can!

## How?

Here's a brief explanation on how lang_it works:

The whole engine exists within the header file, if you want to write an app that uses, you just need to use the compile it against lang_it.h

It has multiple layers that do operations to locate words in a dictionary, apply inflection before and after, apply deterministic rules, convert script, normalize words and etc.

Roughly like this:

`Input > Tokenize > Lookup n-grams [3, 2] > Individual word Lookup > Dictionary Lookup with inflexion > Optional auto-correct > Apply inflection/morphology to target language > Apply deterministic rules > Normalize common patterns > Homonym disambiguation > Output`

lang_it works with binary files (.lang) that encode all dictionaries, rules and transformations, mapping how language A represents a concept vs how language B does the same thing. 

For example, using a json template, you teach the program that portuguese makes plurals by adding "-s" suffixes to nouns, and you also teach it that italian does the same but with the suffix "-i", after that, every dictionary lookup will take this information into account and will be able to locate the plural form as well as the stored root.

The compiler is still super limited, but its my current focus of development right now. There's also a GUI to create modules faster in development but in the moment, you need to create your own json files and compile them manually.

For example, to create a simple Bahasa Malay -> Simplified Chinese, we need a json file like:

ma_zh.json: 
```json
{
  "dictionary": [
    {
      "entry": "kucing",
      "translation": "猫",
      "word_type": 0
    },
    {
      "entry": "anjing",
      "translation": "狗",
      "word_type": 0
    },
    {
      "entry": "air",
      "translation": "水",
      "word_type": 0
    },
     {
      "entry": "cendawan",
      "translation": "蘑菇",
      "word_type": 0
    }
  ]
}
```

You then run the compiler, with the name that you want for your module and the path of your json file:

```sh
./compiler test.lang ma_zh.json
```

That will generate the file `test.lang`


This file can now be loaded (however it is that youre doing it) into the engine using the function `load_from_bin(const uint8_t* file, size_t size)`. You load it however you want, but you need to feed into the function the whole buffered file, and it's size:

C++ example:

```cpp 
    std::ifstream file(argv[2], std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        std::cout << "Failed to read file\n";
        return 1;
    }

    
   load_from_bin(buffer.data(), buffer.size()); //load the buffer and the size

   cout << translate_from_bin("air, chendawan") // then you call the translation
```

WASM Javascript example:

```js
const response = await fetch(fileUrl);
const arrayBuffer = await response.arrayBuffer();
fileBuffer = new Uint8Array(arrayBuffer);
Module.translate_from_bin("air, chendawan", fileBuffer); // in the wasm bindings, you load and translate with the same function, i know its a lof of useless overhead but i'm not well versed enough in wasm stuff to find out a better way, if i keep them separate i lose the context.
```

## Current State

✅ = WORKS ACROSS MULTIPLE LANGUAGES

🆗 = FUNCTIONAL, BUT NOT FULLY

⚠️ = IMPLEMENTED, BUT ONLY FOR SPECIFIC CASES

❌ = NOT IMPLEMENTED

| FEATURE          |  STATUS    |  TESTED WITH | 
| -------------    |  ------    | -----        | 
| Word Lookup      |   ✅       | English, Portuguese, Japanese, Spanish, French, Chinese, Malay, others.|  
| Auto Correct     |   ⚠️       | Portuguese, English|    
| Gender Inflection|   ✅       | Portuguese, Russian, Spanish|    
| Multi Script     |   🆗       | Japanese (Kana/Kanji/Romaji), Chinese (Hanzi/pīnyīn), Malay(Rumi, Jawi)|  
| Grammatical Case |   ⚠️       |   Portuguese, Russian  |   

-----

lang_it Personal Use License v1.0

Permission is granted to use, modify, and distribute this software
for personal, educational, and research purposes.

Commercial use, including but not limited to embedding in products,
offering paid services, or redistribution in commercial contexts,
requires a separate commercial license agreement.