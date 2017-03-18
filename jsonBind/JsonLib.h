#include <string>
#include "json.hpp"


class JsonLib {
	nlohmann::json myJson;
public:
	JsonLib();
	void setJson(std::string jsonString);
	std::string getJson();
	void del(std::string key);
};