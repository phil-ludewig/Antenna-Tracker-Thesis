
void setupSteppers(void)
{
  //elevation motor:
  boardA.configSyncPin(BUSY_PIN, 0);
  boardA.configStepMode(STEP_FS_128);
  boardA.setMaxSpeed(500);
  boardA.setFullSpeed(5000);
  boardA.setMinSpeed(300);
  boardA.setAcc(400);
  boardA.setDec(400);
  boardA.setSlewRate(SR_530V_us);
  boardA.setOCThreshold(OC_4500mA);
  boardA.setPWMFreq(PWM_DIV_2, PWM_MUL_2);
  boardA.setOCShutdown(OC_SD_ENABLE);
  boardA.setVoltageComp(VS_COMP_DISABLE);
  boardA.setSwitchMode(SW_HARD_STOP);
  boardA.setOscMode(INT_16MHZ_OSCOUT_16MHZ);
  boardA.setAccKVAL(200);
  boardA.setDecKVAL(200);
  boardA.setRunKVAL(200);
  boardA.setHoldKVAL(32);



  //azimut motor:
  boardB.configSyncPin(BUSY_PIN, 0);
  boardB.configStepMode(STEP_FS_128);
  boardB.setMaxSpeed(400);
  boardB.setFullSpeed(5000);
  boardB.setAcc(200);
  boardB.setDec(200);
  boardB.setSlewRate(SR_290V_us);
  boardB.setOCThreshold(OC_6000mA);
  boardB.setPWMFreq(PWM_DIV_2, PWM_MUL_2);
  boardB.setOCShutdown(OC_SD_ENABLE);
  boardB.setVoltageComp(VS_COMP_DISABLE);
  boardB.setSwitchMode(SW_HARD_STOP);
  boardB.setOscMode(EXT_16MHZ_OSCOUT_INVERT);
  boardB.setAccKVAL(200);
  boardB.setDecKVAL(200);
  boardB.setRunKVAL(200);
  boardB.setHoldKVAL(10);


}
