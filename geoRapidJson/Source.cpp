#include <emscripten/bind.h>
#include <unordered_map>
#include "rapidjson/document.h"
#include "rapidjson/allocators.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

using namespace emscripten;
using namespace rapidjson;

std::string returnJson(std::string s) {
	Document document;
	document.Parse<0>(s.c_str());

	std::unordered_map<std::string, int> stringCounter;

	const Value& f = document["features"];
	for (Value::ConstValueIterator itf = f.Begin(); itf != f.End(); ++itf) {
		const Value& p = (*itf)["properties"];
		for (Value::ConstMemberIterator itp = p.MemberBegin(); itp != p.MemberEnd(); ++itp)
		{
			itp->name.GetString();
			itp->value.GetString();
			stringCounter[itp->value.GetString()]++;
		}
	}

	Document dRes;
	Document::AllocatorType& alloc = dRes.GetAllocator();
	dRes.SetObject();

	for (auto& pair : stringCounter)
		dRes.AddMember(StringRef(pair.first.c_str()), Value(pair.second), alloc);
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	dRes.Accept(writer);
	return buffer.GetString();
}

EMSCRIPTEN_BINDINGS(my_module) {
    function("returnJson", &returnJson);
}