#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"

//Hardware Setup
#define RFID_INT     8
#define RFID_RST     9
#define RFID_SCK    10
#define RFID_MOSI   11
#define RFID_MISO   12
#define RFID_CS     13

/* //MFRC522 Register Addresses
//Page 0 Command and Status
#define CommandReg      0x01
#define ComlEnReg       0x02
#define DivlEnReg       0x03
#define ComIrqReg       0x04
#define DivIrqReg       0x05
#define ErrorReg        0x06
#define Status1Reg      0x07
#define Status2Reg      0x08
#define FIFODataReg     0x09
#define FIFOLevelReg    0x0A
#define WaterLevelReg   0x0B
#define ControlReg      0x0C
#define BitFramingReg   0x0D
#define CollReg         0x0E

//Page 1 Command
#define ModeReg         0x11
#define TxModeReg       0x12
#define RxModeReg       0x13
#define TxControlReg    0x14
#define TxASKReg        0x15
#define TxSelReg        0x16
#define RxSelReg        0x17
#define RxThresholdReg  0x18
#define DemodReg        0x19
#define MfTxReg         0x1C
#define MfRxReg         0x1D
#define SerialSpeedReg  0x1F

//Page 2 Configuration
#define CRCResultReg1   0x21
#define CRCResultReg2   0x22
#define ModWidthReg     0x24
#define RFCfgReg        0x26
#define GsNReg          0x27
#define CWGsPReg        0x28
#define ModGsPReg       0x29
#define TModeReg        0x2A
#define TPrescalerReg   0x2B
#define TReloadReg1     0x2C
#define TReloadReg2     0x2D
#define TCounterValReg1 0x2E
#define TCounterValReg2 0x2F

//Page 3 Test Register
#define TestSel1Reg     0x31
#define TestSel2Reg     0x32
#define TestPinEnReg    0x33
#define TestPinValueReg 0x34
#define TestBusReg      0x35
#define AutoTestReg     0x36
#define VersionReg      0x37
#define AnalogTestReg   0x38
#define TestDAC1Reg     0x39
#define TestDAC2Reg     0x3A
#define TestADCReg      0x3B */


uint8_t MFRC522_ReadReg(uint8_t RegAddr);
void MFRC522_WriteReg(uint8_t RegAddr, uint8_t Data);

void MFRC522_HW_INIT();
//void MFRC522_INIT();

void MFRC522_AntennaOn();
void MFRC522_AntennaOff();
