#include "nRF24L01.h"
#include "RF24.h"
static char send_payload[256];

const int min_payload_size = 4;
const int max_payload_size = 32;
const int payload_size_increments_by = 1;
int next_payload_size = min_payload_size;

RF24 radio(7, 8); //CE, CSN

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
char receive_payload[max_payload_size + 1];
void setup()
{
  Serial.begin(115200);
  radio.begin();
  // enable dynamic payloads
  radio.enableDynamicPayloads();
  radio.setRetries(5, 15);

  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  radio.startListening();
}

void loop(void)
{
  int water_level = 30;
  Serial.print("Sending Data :");
  Serial.print(water_level);
  Serial.println(" cm");
  delay(100);
  String water = String(water_level);
  static char send_payload[50];
  water.toCharArray(send_payload, 50);
  Serial.println(send_payload);

  // First, stop listening so we can talk.
  radio.stopListening();

  // Take the time, and send it.  This will block until complete
  Serial.print(F("Now sending length "));
  Serial.println(next_payload_size);
  radio.write( send_payload, next_payload_size );
  // Now, continue listening
  radio.startListening();
}
