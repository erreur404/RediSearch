
#pragma once

#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////////////////////

enum RSLanguage {
  RS_LANG_ENGLISH = 0,
  RS_LANG_ARABIC,
  RS_LANG_CHINESE,
  RS_LANG_DANISH,
  RS_LANG_DUTCH,
  RS_LANG_FINNISH,
  RS_LANG_FRENCH,
  RS_LANG_GERMAN,
  RS_LANG_HINDI,
  RS_LANG_HUNGARIAN,
  RS_LANG_ITALIAN,
  RS_LANG_NORWEGIAN,
  RS_LANG_PORTUGUESE,
  RS_LANG_ROMANIAN,
  RS_LANG_RUSSIAN,
  RS_LANG_SPANISH,
  RS_LANG_SWEDISH,
  RS_LANG_TAMIL,
  RS_LANG_TURKISH,
  RS_LANG_UNSUPPORTED
};

//---------------------------------------------------------------------------------------------

enum StemmerType {
  SnowballStemmer
};

#define DEFAULT_LANGUAGE RS_LANG_ENGLISH
#define STEM_PREFIX '+'
#define STEMMER_EXPANDER_NAME "stem"

//---------------------------------------------------------------------------------------------

// Abstract "interface" for a pluggable stemmer, ensuring we can use multiple stemmer libs
class Stemmer : Object {
  void __newSnowballStemmer(RSLanguage language);
  int __sbstemmer_Reset(StemmerType type, RSLanguage language);
public:
  void *ctx;
  const char *(*Stem)(void *ctx, const char *word, size_t len, size_t *outlen);

  Stemmer();
  virtual ~Stemmer();

  // Attempts to reset the stemmer using the given language and type. Returns 0
  // if this stemmer cannot be reused.
  // int (*Reset)(struct stemmer *, StemmerType type, RSLanguage language);
  virtual int Reset(StemmerType type, RSLanguage language);

  RSLanguage language;
  StemmerType type;  // Type of stemmer
};

//---------------------------------------------------------------------------------------------


// check if a language is supported by our stemmers
RSLanguage RSLanguage_Find(const char *language);
const char *RSLanguage_ToString(RSLanguage language);

// Get a stemmer expander instance for registering it
void RegisterStemmerExpander();

// Snoball Stemmer wrapper implementation
const char *__sbstemmer_Stem(void *ctx, const char *word, size_t len, size_t *outlen);

///////////////////////////////////////////////////////////////////////////////////////////////
