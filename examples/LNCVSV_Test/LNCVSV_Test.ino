#include <EEPROM.h>
#include <LocoNet.h>
#include <LNCVDefaultStorage.h>

#define DO_DEBUG

const uint16_t artNr = 4321;

LocoNetSystemVariableClass sv;
lnMsg       *LnPacket;
SV_STATUS   svStatus = SV_OK;
boolean     deferredProcessingNeeded = false;

void setup() {
  Serial.begin(115200);
  LocoNet.init(); 
  sv.init(13, 42, 9685, 1);

  sv.writeSVStorage(SV_ADDR_NODE_ID_H, 1 );
  sv.writeSVStorage(SV_ADDR_NODE_ID_L, 0);
  
  sv.writeSVStorage(SV_ADDR_SERIAL_NUMBER_H, 0xA1);
  sv.writeSVStorage(SV_ADDR_SERIAL_NUMBER_L, 0xA2);

  Serial.println("LNCVSV_Test Ready");
}

uint32_t count = 0;
void loop() {
  if((count % 0x00800000) == 0 ) {
    Serial.print("Main Loop: ");
    Serial.println(millis());
  }
  LnPacket = LocoNet.receive();
  svStatus = sv.processMessage(LnPacket);
  
  deferredProcessingNeeded = (svStatus == SV_DEFERRED_PROCESSING_NEEDED);
  
  if(deferredProcessingNeeded)
    deferredProcessingNeeded = (sv.doDeferredProcessing() != SV_OK);
  count++;
}

void printId(uint16_t an, uint16_t ad) {
  Serial.print(an);
  Serial.print(", ");
  Serial.println(ad);
}

void notifySVChanged(uint16_t Offset) {
  Serial.print("SV Changed: ");
  Serial.print(Offset);
  Serial.print(" = ");
  Serial.println(sv.readSVStorage(Offset));
}
