/*
    Daikin heatpump control (Daikin remote control P/N ARC480A11)
*/
#ifndef DaikinHeatpumpARC480IR_h
#define DaikinHeatpumpARC480IR_h

#include <HeatpumpIR.h>


// Daikin timing constants
#define DAIKIN_AIRCON2_HDR_MARK   3530
#define DAIKIN_AIRCON2_HDR_SPACE  1325
#define DAIKIN_AIRCON2_BIT_MARK   391
#define DAIKIN_AIRCON2_ONE_SPACE  1325
#define DAIKIN_AIRCON2_ZERO_SPACE 405
#define DAIKIN_AIRCON2_MSG_SPACE  25720

// Daikin codes
#define DAIKIN_AIRCON2_MODE_AUTO  0x01 // Operating mode
#define DAIKIN_AIRCON2_MODE_HEAT  0x41
#define DAIKIN_AIRCON2_MODE_COOL  0x31
#define DAIKIN_AIRCON2_MODE_DRY   0x21
#define DAIKIN_AIRCON2_MODE_FAN   0x61
#define DAIKIN_AIRCON2_MODE_OFF   0x40 // Power
#define DAIKIN_AIRCON2_MODE_ON    0x00
#define DAIKIN_AIRCON2_FAN_AUTO   0xA0 // Fan speed
#define DAIKIN_AIRCON2_FAN_QUIET  0xB0 // Fan speed
#define DAIKIN_AIRCON2_FAN1       0x30
#define DAIKIN_AIRCON2_FAN2       0x40
#define DAIKIN_AIRCON2_FAN3       0x50
#define DAIKIN_AIRCON2_FAN4       0x60
#define DAIKIN_AIRCON2_FAN5       0x70
#define DAIKIN_AIRCON2_POWERFUL   0x01 // FLAGS
#define DAIKIN_AIRCON2_SWING      0x0F
#define DAIKIN_AIRCON2_ECONO      0x04
#define DAIKIN_AIRCON2_CONFORT    0x02
#define DAIKIN_AIRCON2_ECOCONFORT 0x06


class DaikinHeatpumpARC480IR : public HeatpumpIR
{
  public:
    DaikinHeatpumpARC480IR();
    void send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd, uint8_t swingHCmd);

  private:
    void sendDaikin(IRSender& IR, uint8_t powerMode, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature, uint8_t swingV, uint8_t swingH);
};

#endif