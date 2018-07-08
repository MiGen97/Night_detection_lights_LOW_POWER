# Night_detection_lights_LOW_POWER
Turn on the lights for a short period of time only if outside is night


This code is written for Attiny85. The watchdog functionality is take from here: https://gist.github.com/dwhacks/8055287.

Description of application:
  When outside is dark, at every 30 minutes (aprox.) turn on the lights for about 2 minutes.
  
Used components:
  Attiny85;
  Relay module;
  Photoresistor;
  Battery pack.
