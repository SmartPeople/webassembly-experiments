#include <set>
#include <string>
#include <unordered_map>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <emscripten/bind.h>

using namespace std;
using namespace rapidjson;

struct ForJson {
	wstring group;
	wstring key;
};

struct Counter {
	wstring group;
	unordered_map<wstring, int> mapCounter;
};


wstring returnJson(wstring ss) {
	
	ForJson fj;
	Counter counter;
	vector<ForJson> v;
	
	typedef rapidjson::GenericDocument<rapidjson::UTF16<>> WDocument;
	typedef rapidjson::GenericValue<rapidjson::UTF16<> > WValue;
	typedef rapidjson::GenericStringBuffer< rapidjson::UTF16<> > StringBuffer_UTF16;
	
	WDocument document;
	document.Parse(ss.c_str());
	
	
	const WValue& f = document[L"features"];
	for (WValue::ConstValueIterator itf = f.Begin(); itf != f.End(); ++itf) {
		const WValue& p = (*itf)[L"properties"];
		for (WValue::ConstMemberIterator itp = p.MemberBegin(); itp != p.MemberEnd(); ++itp)
		{
			fj.group = itp->name.GetString();
			fj.key = itp->value.GetString();
			v.push_back(fj);
		}
	}

	set<wstring> uniq;
	for (auto &i : v) {
		uniq.insert(i.group);
	}
	
	unordered_map<wstring, int> stringCounter;
	StringBuffer_UTF16 s;
	Writer<StringBuffer_UTF16, UTF16<>, UTF16<>> writer(s);
	
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