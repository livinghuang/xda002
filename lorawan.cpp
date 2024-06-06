#include "global.h"

static void prepareTxFrame(uint8_t port);
/* OTAA para*/
uint8_t devEui[] = {0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x01};
uint8_t appEui[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t appKey[] = {0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88};

/* ABP para*/
uint8_t nwkSKey[] = {0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88};
uint8_t appSKey[] = {0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88};
uint32_t devAddr = (uint32_t)0x88888801;
/*LoraWan channelsmask, default channels 0-7*/
uint16_t userChannelsMask[6] = {0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};

/*LoraWan region,
    LORAMAC_REGION_AS923        //  AS band on 923MHz
    LORAMAC_REGION_AU915        //  Australian band on 915MHz
    LORAMAC_REGION_CN470        //  Chinese band on 470MHz
    LORAMAC_REGION_CN779        //  Chinese band on 779MHz
    LORAMAC_REGION_EU433        //  European band on 433MHz
    LORAMAC_REGION_EU868        //  European band on 868MHz
    LORAMAC_REGION_KR920        //  South korean band on 920MHz
    LORAMAC_REGION_IN865        //  India band on 865MHz
    LORAMAC_REGION_US915        //  North american band on 915MHz
    LORAMAC_REGION_US915_HYBRID //  North american band on 915MHz with a maximum of 16 channels
    LORAMAC_REGION_AU915_SB2    //  Australian band on 915MHz Subband 2
    LORAMAC_REGION_AS923_AS1    //  AS band on 922.0-923.4MHz
    LORAMAC_REGION_AS923_AS2    //  AS band on 923.2-924.6MHz
*/
LoRaMacRegion_t loraWanRegion = LORAMAC_REGION_AS923_AS2;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t loraWanClass = CLASS_A;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 300000;

/*OTAA or ABP*/
bool overTheAirActivation = false;

/*ADR enable*/
bool loraWanAdr = true;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = true;

/* Application port */
uint8_t appPort = 2;
/*!
 * Number of trials to transmit the frame, if the LoRaMAC layer did not
 * receive an acknowledgment. The MAC performs a datarate adaptation,
 * according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
 * to the following table:
 *
 * Transmission nb | Data Rate
 * ----------------|-----------
 * 1 (first)       | DR
 * 2               | DR
 * 3               | max(DR-1,0)
 * 4               | max(DR-1,0)
 * 5               | max(DR-2,0)
 * 6               | max(DR-2,0)
 * 7               | max(DR-3,0)
 * 8               | max(DR-3,0)
 *
 * Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
 * the datarate, in case the LoRaMAC layer did not receive an acknowledgment
 */
uint8_t confirmedNbTrials = 4;

void lorawan_init(void)
{
#if (LORAWAN_DEVEUI_AUTO)
    LoRaWAN.generateDeveuiByChipID();
#endif
    Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);
    deviceState = DEVICE_STATE_INIT;
}

void lorawan_process(void)
{
    switch (deviceState)
    {
    case DEVICE_STATE_INIT:
    {
#if (LORAWAN_DEVEUI_AUTO)
        LoRaWAN.generateDeveuiByChipID();
#endif
        LoRaWAN.init(loraWanClass, loraWanRegion);
        // both set join DR and DR when ADR off
        LoRaWAN.setDefaultDR(3);
        break;
    }
    case DEVICE_STATE_JOIN:
    {
        LoRaWAN.join();
        break;
    }
    case DEVICE_STATE_SEND:
    {
        prepareTxFrame(appPort);
        LoRaWAN.send();
        deviceState = DEVICE_STATE_CYCLE;
        break;
    }
    case DEVICE_STATE_CYCLE:
    {
        // Schedule next packet transmission
        txDutyCycleTime = appTxDutyCycle + randr(-APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND);
        LoRaWAN.cycle(txDutyCycleTime);
        deviceState = DEVICE_STATE_SLEEP;
        break;
    }
    case DEVICE_STATE_SLEEP:
    {
        LoRaWAN.sleep(loraWanClass);
        pinMode(pVext, OUTPUT);
        digitalWrite(pVext, HIGH);
        break;
    }
    default:
    {
        deviceState = DEVICE_STATE_INIT;
        break;
    }
    }
}

static void prepareTxFrame(uint8_t port)
{
    Serial.println("Prepare Frame");
    Serial.flush();
}
