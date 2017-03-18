#include <emscripten.h>
#include <string>
#include "json.hpp"
using json = nlohmann::json;


extern "C" {
                                       

// void pointer_callback(void(*f)(void)) {
// 	(*f)();
// }



void sendMsg(const std::string& msg)
{
    EM_ASM_ARGS(
        {
        	//apply to some var
            console.log(Pointer_stringify($0));
        },
        msg.c_str()
    );
}

void sendJson(){
	json j;
    j["wasm"] = "emscripten";
    j["number"] = 42;
	sendMsg(j.dump());
}

void printTitle() {
	EM_ASM(
		console.log(document.title);
	);
}

}