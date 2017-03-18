#include "JsonLib.h"
#include <iostream>
using json = nlohmann::json;


JsonLib::JsonLib() {}

void JsonLib::setJson(std::string jsonString) {
	myJson = json::parse(jsonString);
}

std::string JsonLib::getJson() {
	return myJson.dump();
}

void JsonLib::del(std::string key) {
	myJson.erase(key);
}