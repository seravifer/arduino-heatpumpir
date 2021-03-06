#include <DaikinHeatpumpARC480IR.h>

DaikinHeatpumpARC480IR::DaikinHeatpumpARC480IR() : HeatpumpIR()
{
  static const char PROGMEM model[] PROGMEM = "daikin_arc480";
  static const char PROGMEM info[] PROGMEM = "{\"mdl\":\"daikin_arc480\",\"dn\":\"Daikin ARC480\",\"mT\":18,\"xT\":30,\"fs\":6,\"maint\":[10,11,12,13,14,15,16,17]}}}";

  _model = model;
  _info = info;
}

// Daikin numeric values to command bytes
void DaikinHeatpumpARC480IR::send(IRSender &IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, int temperatureCmd, uint8_t fanSpeedCmd,
                                  bool econoCmd, bool powerfullCmd, bool comfortCmd, bool swingCmd)
{
  // Defaults
  uint8_t powerMode = DAIKIN_AIRCON2_MODE_OFF;
  uint8_t operatingMode = DAIKIN_AIRCON2_MODE_AUTO;
  uint8_t fanSpeed = DAIKIN_AIRCON2_FAN_AUTO;
  uint8_t temperature = 0x28; // 20 deg
  uint8_t powerfull = 0x00;

  if ((operatingModeCmd == MODE_HEAT && temperatureCmd >= 14 && temperatureCmd <= 28) ||
      (operatingModeCmd == MODE_COOL && temperatureCmd >= 18 && temperatureCmd <= 32) ||
      (temperatureCmd >= 18 && temperatureCmd <= 28))
  {
    temperature = (uint8_t)temperatureCmd * 2;
  }

  switch (powerModeCmd)
  {
  case POWER_ON:
    powerMode = DAIKIN_AIRCON2_MODE_ON;
    break;

  case POWER_OFF:
    powerMode = DAIKIN_AIRCON2_MODE_OFF;
    break;
  }

  switch (operatingModeCmd)
  {
  case MODE_AUTO:
    operatingMode = DAIKIN_AIRCON2_MODE_AUTO;
    break;
  case MODE_HEAT:
    operatingMode = DAIKIN_AIRCON2_MODE_HEAT;
    break;
  case MODE_COOL:
    operatingMode = DAIKIN_AIRCON2_MODE_COOL;
    break;
  case MODE_DRY:
    operatingMode = DAIKIN_AIRCON2_MODE_DRY;
    // temperature = 0x80;
    break;
  case MODE_FAN:
    operatingMode = DAIKIN_AIRCON2_MODE_FAN;
    // temperatureCmd = 0xC0;
    break;
  }

  switch (fanSpeedCmd)
  {
  case FAN_AUTO:
    fanSpeed = DAIKIN_AIRCON2_FAN_AUTO;
    break;
  case FAN_QUIET:
    fanSpeed = DAIKIN_AIRCON2_FAN_QUIET;
    break;
  case FAN_1:
    fanSpeed = DAIKIN_AIRCON2_FAN1;
    break;
  case FAN_2:
    fanSpeed = DAIKIN_AIRCON2_FAN2;
    break;
  case FAN_3:
    fanSpeed = DAIKIN_AIRCON2_FAN3;
    break;
  case FAN_4:
    fanSpeed = DAIKIN_AIRCON2_FAN4;
    break;
  case FAN_5:
    fanSpeed = DAIKIN_AIRCON2_FAN5;
    break;
  }

  if (swingCmd)
    fanSpeed += 0x0F;

  if (powerModeCmd == POWER_ON)
  {
    if (comfortCmd)
      powerMode = 0x02;
    if (econoCmd)
      powerMode = 0x04;
    if (econoCmd && comfortCmd)
      powerMode = 0x06;
  }

  if (powerfullCmd)
    powerfull = 0x01;

  sendDaikin(IR, powerMode, operatingMode, fanSpeed, temperature, powerfull);
}

// Send the Daikin code
void DaikinHeatpumpARC480IR::sendDaikin(IRSender &IR, uint8_t powerMode, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature, uint8_t powerfull)
{
  uint8_t daikinTemplate[] = {
      0x11, 0xDA, 0x27, 0x00, 0x00, // Header
      // 0     1     2     3     4
      0x01, 0x34, 0x00, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC5, 0x00, 0x00, 0x1D};
      // 5     6     7     8     9    10    11    12    13    14    15    16    17    18

  daikinTemplate[5] = operatingMode;
  daikinTemplate[6] = temperature;
  daikinTemplate[8] = fanSpeed;
  daikinTemplate[12] = powerfull;
  daikinTemplate[16] = powerMode;

  // Checksum calculation
  uint8_t checksum = 0x00;

  for (int i = 0; i < 18; i++)
  {
    checksum += daikinTemplate[i];
  }

  checksum = checksum & 0xff;
  daikinTemplate[18] = checksum;

  // 38 kHz PWM frequency
  IR.setFrequency(38);

  // Header
  for (int i = 0; i < 5; i++)
  {
    IR.mark(DAIKIN_AIRCON2_BIT_MARK);
    IR.space(DAIKIN_AIRCON2_ZERO_SPACE);
  }

  IR.mark(DAIKIN_AIRCON2_BIT_MARK);
  IR.space(DAIKIN_AIRCON2_MSG_SPACE);
  IR.mark(DAIKIN_AIRCON2_HDR_MARK);
  IR.space(DAIKIN_AIRCON2_HDR_SPACE);

  for (int i = 0; i <= 18; i++)
  {
    IR.sendIRbyte(daikinTemplate[i], DAIKIN_AIRCON2_BIT_MARK, DAIKIN_AIRCON2_ZERO_SPACE, DAIKIN_AIRCON2_ONE_SPACE);
  }

  IR.mark(DAIKIN_AIRCON2_BIT_MARK);
  IR.space(0);
}