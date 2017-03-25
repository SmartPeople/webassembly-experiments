#include <string>
#include <unordered_map>
#include <set>
#include "rapidjson/document.h"
#include "rapidjson/allocators.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <emscripten/bind.h>

using namespace rapidjson;
using namespace std;


struct ForJson {
	string group;
	string key;
};

struct Counter {
	string group;
	unordered_map<string, int> mapCounter;
};

string returnJson(string doc) {
	Document document;
	document.Parse<0>(doc.c_str());

	ForJson fj;
	Counter counter;
	vector<ForJson> v;


	const Value& f = document["features"];
	for (Value::ConstValueIterator itf = f.Begin(); itf != f.End(); ++itf) {
		const Value& p = (*itf)["properties"];
		for (Value::ConstMemberIterator itp = p.MemberBegin(); itp != p.MemberEnd(); ++itp)
		{
			fj.group = itp->name.GetString();
			fj.key = itp->value.GetString();
			v.push_back(fj);
		}
	}

	set<string> uniq;
	for (auto &i : v) {
		uniq.insert(i.group);
	}


	unordered_map<std::string, int> stringCounter;

	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();


	for (auto &uu : uniq) {
		stringCounter.clear();
		writer.Key(uu.c_str());


		for (auto &vv : v) {
			if (vv.group == uu)
			{
				stringCounter[vv.key]++;
			}
		}

		writer.StartObject();
		for (auto &sc : stringCounter)
		{
			writer.Key(sc.first.c_str());
			writer.Uint(sc.second);
		}
		writer.EndObject();

	}

	writer.EndObject();
	return s.GetString();
}

namespace emscripten {
	EMSCRIPTEN_BINDINGS(my_module) {
		function("returnJson", &returnJson);
	}
}