void Pin_SetUp(uint8_t __FirstPin, uint8_t __LastPin){
  for (uint8_t i = __FirstPin; i <= __LastPin; i++){
    pinMode(i, OUTPUT); digitalWrite(i, HIGH);
  };
};

void SwitchOnOffModule(bool __COMMAND_SWICTH_ON, uint8_t __Switch_PIN, bool &__SWITCHED_PARAMETER){
  if(__COMMAND_SWICTH_ON){
    digitalWrite(__Switch_PIN,LOW);
    __SWITCHED_PARAMETER = true;
  }else{
    digitalWrite(__Switch_PIN,HIGH);
    __SWITCHED_PARAMETER = false;
  };
};
