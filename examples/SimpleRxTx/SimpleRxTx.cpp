/*
 * Zeta RF Getting Started Code Example.
 * Basic example on how to send messages back and forth between two modules.
 *
 * Usage: write this sample on both boards and send text over serial!
 */

#define ZETARF_DEBUG_ON

#include <iostream>
#include <wiringPi.h>
#include <ZetaRF.h>

// Zeta modules transmit messages using fixed size packets, define here the max size you want to use
#define ZETARF_PACKET_LENGTH 16

ZetaRF433<ChipSelectPin<6>, ShutdownPin<9>, IrqPin<8>> zeta;

char data[ZETARF_PACKET_LENGTH] = "Hello ";
bool transmitting = false;


bool setup()
{
  delay(1000);

  std::cout << "Starting Zeta TxRx..." << std::endl;

  // Initialize Zeta module, specifing channel and packet size
  if (!zeta.beginWithPacketLengthOf(ZETARF_PACKET_LENGTH)) {
    std::cout << "Zeta begin failed" << std::endl;
    return false;
  }

  // Print some info about the chip
  const Si4455_PartInfo &pi = zeta.readPartInformation();
  std::cout << "----------" << std::endl;
  std::cout << "Chip rev: " << (int) pi.CHIPREV  << std::endl;
  std::cout << "Part    : " <<       pi.PART.U16 << std::endl;
  std::cout << "PBuild  : " << (int) pi.PBUILD   << std::endl;
  std::cout << "ID      : " <<       pi.ID.U16   << std::endl;
  std::cout << "Customer: " << (int) pi.CUSTOMER << std::endl;
  std::cout << "Rom ID  : " << (int) pi.ROMID    << std::endl;
  std::cout << "Bond    : " << (int) pi.BOND     << std::endl;
  std::cout << std::endl;

  const Si4455_FuncInfo &fi = zeta.readFunctionRevisionInformation();
  std::cout << "Rev Ext   : " << (int) fi.REVEXT     << std::endl;
  std::cout << "Rev Branch: " << (int) fi.REVBRANCH  << std::endl;
  std::cout << "Rev Int   : " << (int) fi.REVINT     << std::endl;
  std::cout << "Patch     : " << (int) fi.PATCH.U16  << std::endl;
  std::cout << "Func      : " << (int) fi.FUNC       << std::endl;
  std::cout << "SVN Flags : " << (int) fi.SVNFLAGS   << std::endl;
  std::cout << "SVN Rev   : " <<       fi.SVNREV.U32 << std::endl;
  std::cout << "----------" << std::endl; //*/
  
  zeta.startListeningOnChannel(4);

  std::cout << "Init done." << std::endl;
  return true;
}



void loop()
{
  // @todo Haven't worked out how to do this bit yet.
  // Requires simultaneous input/output from standard io.

  // Send any data received from serial
  /*if (Serial.available() && !transmitting) {
    int s = Serial.readBytes(data, ZETARF_PACKET_LENGTH);

    // Pad with zeros
    for (int i = s; i < ZETARF_PACKET_LENGTH; i++) {
      data[i] = 0;
    }

    Serial.print("Sending >");
    Serial.write(data, ZETARF_PACKET_LENGTH);
    Serial.print("<\n");

    // Send buffer
    transmitting = true;  // Only one at a time!
    zeta.sendPacket((const uint8_t*)data);  // Use channel set with begin()
    // Module will automatically return to listening mode
  }

  // Check if message was transmitted successfully
  if (zeta.checkTransmitted()) {
    transmitting = false;
    Serial.println("msg transmitted");
  }//*/

  // Check incoming messages and print
  if (zeta.checkReceived()) {
    std::cout << "> ";
    zeta.readFixedLengthPacket((uint8_t*)data, ZETARF_PACKET_LENGTH);
    uint8_t* data_ = (uint8_t*)data;
    for (int i = 0; i < ZETARF_PACKET_LENGTH; i++) {
      std::cout << *data_++;
    }
    std::cout << std::endl;
  }//*/
  
  delay(10);
}

int main() {
    if (!setup()) return 1;

    while (true) loop();
}
