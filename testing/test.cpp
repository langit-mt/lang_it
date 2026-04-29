#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "../src/lang_it.h"


TEST_CASE("Irregular verbs from portuguese to english") {
    CHECK(translate("comeu", "pt", "en", 0) == "ate");
    CHECK(translate("matou", "pt", "en", 0) == "killed");
    CHECK(translate("comprou", "pt", "en", 0) == "bought");
    CHECK(translate("alimentou", "pt", "en", 0) == "fed");
    CHECK(translate("bebeu", "pt", "en", 0) == "drank");
    CHECK(translate("escreveu", "pt", "en", 0) == "wrote");
 
}
TEST_CASE("Irregular plural from portuguese to english") {
    CHECK(translate("dentes", "pt", "en", 0) == "teeth");
    CHECK(translate("homens", "pt", "en", 0) == "men");
    CHECK(translate("peixes", "pt", "en", 0) == "fish");
}