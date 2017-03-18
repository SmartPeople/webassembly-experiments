#include "JsonLib.h"
#include "emscripten/bind.h"
using namespace emscripten;

EMSCRIPTEN_BINDINGS(JsonLib) {
    class_<JsonLib>("JsonLib").constructor()
      .function("getJson", &JsonLib::getJson)
      .function("setJson", &JsonLib::setJson)
      .function("del", &JsonLib::del)
      ;

   }