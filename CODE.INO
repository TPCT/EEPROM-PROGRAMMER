#include <math.h>
#include <stdarg.h>
#define WEPIN 41
#define OEPIN 39                                                
struct addresses_pins{
    unsigned short *pins;
    unsigned char count;
};

struct io_pins{
    unsigned short *pins;
    unsigned char count;
};
struct data_vector{
    unsigned int *data;
    unsigned int count;
};

void WriteEEprom(unsigned int, unsigned char, struct addresses_pins, struct data_vector, struct io_pins, struct data_vector);
void ReadEEprom(unsigned int, unsigned char, struct addresses_pins, struct io_pins);
void EraseEEprom(unsigned char, unsigned char, struct addresses_pins, struct io_pins);
void HandlePins(unsigned char, unsigned char, struct addresses_pins, struct io_pins);
void setup(){
    Serial.begin(9600);
    unsigned short addresses_pins_array[] = {22, 24, 26, 28, 30, 32, 34, 36};/* address pins onboard */
    struct addresses_pins Address_Pins = { addresses_pins_array, 8 /*count of address pins*/};
    unsigned short io_pins_array[] = {23, 25, 27, 29, 31, 33, 35, 37}; /*io pins onborad*/
    struct io_pins IO_Pins = {io_pins_array, 8/*io pins count*/};
    unsigned short addresses_array[] = {23, 25, 27, 29, 31, 33, 35, 37}; //addresses to write in
    struct data_vector addresses = {addresses_array, 8};
    //EraseEEprom(WEPIN, OEPIN, Address_Pins, IO_Pins);
    //WriteEEprom(WEPIN, OEPIN, Address_Pins, addresses, IO_Pins, addresses);
    //ReadEEprom(WEPIN, OEPIN, Address_Pins, IO_Pins);
}
void loop(){}
void HandlePins(unsigned char WE, unsigned char OE, struct addresses_pins Address_pins, struct io_pins IO_Pins){
    pinMode(OE, OUTPUT);
    pinMode(WE, OUTPUT);
    for(int i = 0; i < Address_pins.count; i++)
        pinMode(Address_pins.pins[i], OUTPUT);
    for(int i = 0; i < IO_Pins.count; i++)
        pinMode(IO_Pins.pins[i], OUTPUT);
    digitalWrite(OE, HIGH);
    digitalWrite(WE, HIGH);
}
void ReadEEprom(unsigned int WE, unsigned char OE, struct addresses_pins Address_pins, struct io_pins IO_Pins){
    HandlePins(WE, OE, Address_pins, IO_Pins);
    for(int i = 0; i < pow(2, Address_pins.count); ++i){
        Serial.print("Address [");
        Serial.print(i);
        Serial.print("] : ");
        for(int j = 0; j < Address_pins.count; j++){
              digitalWrite(Address_pins.pins[j], i >> j & 1);
              Serial.print(i >> (Address_pins.count - (j+1)) & 1);
        }
        Serial.print(" -> ");
        digitalWrite(WE, HIGH);
        for(int k = 0; k < 200; k++);
        digitalWrite(OE, LOW);
        delayMicroseconds(10);
        for(int j = IO_Pins.count - 1; j >= 0; j--){
              pinMode(IO_Pins.pins[j], INPUT);
              for(int k = 0; k < 200; k++);
              Serial.print(digitalRead(IO_Pins.pins[j]));
              pinMode(IO_Pins.pins[j], OUTPUT);
        }
        Serial.println();
        delayMicroseconds(1);
        digitalWrite(OE, HIGH);
        delayMicroseconds(250);
   }
}
void EraseEEprom(unsigned char WE, unsigned char OE, struct addresses_pins Address_pins, struct io_pins IO_Pins)
{
    HandlePins(WE, OE, Address_pins, IO_Pins);
    for(int i = 0; i < pow(2, Address_pins.count); ++i){
        for(int j = 0; j < Address_pins.count; j++){
              digitalWrite(Address_pins.pins[j], i >> j & 1);
              for(int k = 0; k < 100; k++);
        }
        for(int k = 0; k < 1000; k++);
        for(int j = 0; j < IO_Pins.count; j++){
              for(int k = 0; k < 200; k++);
              digitalWrite(IO_Pins.pins[j], HIGH);
        }
        for(int k = 0; k < 1000; k++);
        digitalWrite(WE, LOW);
        for(int k = 0; k < 600; k++);
        digitalWrite(WE, HIGH);
        delayMicroseconds(250);
   }
}


void WriteEEprom(unsigned int WE, unsigned char OE, struct addresses_pins Address_pins, struct data_vector addresses, struct io_pins IO_Pins, struct data_vector IO_Data){
    HandlePins(WE, OE, Address_pins, IO_Pins);
    for(int i = 0; i < addresses.count; ++i){
        for(int j = 0; j < Address_pins.count; j++){
              digitalWrite(Address_pins.pins[j], addresses.data[i] >> j & 1);
              for(int k = 0; k < 100; k++);
        }
        for(int k = 0; k < 1000; k++);
        for(int j = 0; j < IO_Pins.count; j++){
              digitalWrite(IO_Pins.pins[j], IO_Data.data[i] >> j & 1);
              for(int k = 0; k < 200; k++);
        }
        for(int k = 0; k < 1000; k++);
        digitalWrite(WE, LOW);
        for(int k = 0; k < 600; k++);
        digitalWrite(WE, HIGH);
        delayMicroseconds(250);
   }
}
