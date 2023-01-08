// PIF_TOOL_CHAIN_OPTION: EXTRA_LIBS := ArduinoLibs
// PIF_TOOL_CHAIN_OPTION: UPLOAD_OPTIONS := -c "cr"

#include "simpleSerial/simpleSerial.h"

#ifdef __cplusplus
extern "C"{
	void setup();
	void loop();
} // extern "C"
#endif

void setup() {
	serialBegin(115200, SERIAL_8N1);
	serialPrintf("Test ...\n");
}

int n = 0;
void loop() {
	while(!serialAvailable());
	int c = serialRead();
	serialPrintf("loop %d : %c:%x\n", n, c, c);
	n++;
}
