#pragma once

#include "wled.h"
#include <Arduino.h>
#include "ezButton.h"

#ifndef BTN_WLAN_TOGGLE
  #define BTN_WLAN_TOGGLE 14
#endif

#ifndef BLUE_LED
  #define BLUE_LED 15
#endif

ezButton Btn_Wlan_Toggle(BTN_WLAN_TOGGLE, INTERNAL_PULLUP); // pinMode is defined in ezButton class constructor

class WiFi_SwitchOff : public Usermod {
  private:    
    wl_status_t wiFiStatus;
  public:  
    
    void setup() {   
      pinMode(BLUE_LED, OUTPUT);
      
      for (uint8_t i=0; i<10; i++) {
        digitalWrite(BLUE_LED, HIGH);
        delay(50);
        digitalWrite(BLUE_LED, LOW);
        delay(50);
      }
      Btn_Wlan_Toggle.setDebounceTime(50);            
    }

    void loop() {      
      Btn_Wlan_Toggle.loop(); //must be called first

       wiFiStatus = WiFi.status();
      
      // Set WLAN to sleep when Button is pressed
      if( (Btn_Wlan_Toggle.isPressed()) && (wiFiStatus == WL_CONNECTED) ) {
        WiFi.disconnect();
        WiFi.mode(WIFI_OFF);        
        while (WiFi.status() == WL_CONNECTED) {} // wait until disconnected
        digitalWrite(BLUE_LED, LOW); // Turn off Blue LED
      }
      // Wake up WLAN, when Button is released or not connected (Default State, using internap pullup)
      else if( (Btn_Wlan_Toggle.isReleased()) && (wiFiStatus == WL_DISCONNECTED) ) {
        WiFi.mode(WIFI_STA);
        digitalWrite(BLUE_LED, HIGH); // Turn on Blue LED        
      }
      
    }
};
