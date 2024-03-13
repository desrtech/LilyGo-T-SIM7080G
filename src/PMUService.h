#include <Arduino.h>
#define XPOWERS_CHIP_AXP2101
#include <XPowersLib.h>
#include <utilities.h>

XPowersPMU PMU;

bool pmu_flag = false;
bool led_state = false;

uint32_t loopMillis;
uint32_t loopMillis2;

void setFlag(void) {
    pmu_flag = true;
}

void iniciaXPowers() {
    Serial.println("Iniciando PMU");
    delay(1000);
    Serial.println();
    bool resPmu;
    resPmu = PMU.begin(Wire1, AXP2101_SLAVE_ADDRESS, I2C_SDA, I2C_SCL);
    // Serial.println(resPmu);


    if(!resPmu) {
        Serial.println("Failed to initialize power.....");
        while(1) {
            delay(5000);
        }
    }

    int sda = 13;
    int scl = 21;
    Wire.begin(sda, scl);
    Serial.printf("getID:0x%x\n", PMU.getChipID());
    PMU.setVbusVoltageLimit(XPOWERS_AXP2101_VBUS_VOL_LIM_4V36);
    PMU.setVbusCurrentLimit(XPOWERS_AXP2101_VBUS_CUR_LIM_1500MA);
    PMU.setSysPowerDownVoltage(2600);
    PMU.setDC3Voltage(3000);    //SIM7080 Modem main power channel 2700~ 3400V
    PMU.enableDC3();

    PMU.setBLDO2Voltage(3300);
    PMU.enableBLDO2();
    PMU.disableTSPinMeasure();

    PMU.enableBattDetection();
    PMU.enableVbusVoltageMeasure();
    PMU.enableBattVoltageMeasure();
    PMU.enableSystemVoltageMeasure();

    pinMode(PMU_INPUT_PIN, INPUT);
    attachInterrupt(PMU_INPUT_PIN, setFlag, FALLING);

    // Disable all interrupts
    PMU.disableIRQ(XPOWERS_AXP2101_ALL_IRQ);
    // Clear all interrupt flags
    PMU.clearIrqStatus();
    // Enable the required interrupt function
    PMU.enableIRQ(
        XPOWERS_AXP2101_BAT_INSERT_IRQ    | XPOWERS_AXP2101_BAT_REMOVE_IRQ      |   //BATTERY
        XPOWERS_AXP2101_VBUS_INSERT_IRQ   | XPOWERS_AXP2101_VBUS_REMOVE_IRQ     |   //VBUS
        XPOWERS_AXP2101_PKEY_SHORT_IRQ    | XPOWERS_AXP2101_PKEY_LONG_IRQ       |   //POWER KEY
        XPOWERS_AXP2101_BAT_CHG_DONE_IRQ  | XPOWERS_AXP2101_BAT_CHG_START_IRQ       //CHARGE
    );

    /*********************************
     * step 4 : Set PMU Charger params
    ***********************************/
    // Set the precharge charging current
    PMU.setPrechargeCurr(XPOWERS_AXP2101_PRECHARGE_50MA);
    // Set constant current charge current limit
    PMU.setChargerConstantCurr(XPOWERS_AXP2101_CHG_CUR_200MA);
    // Set stop charging termination current
    PMU.setChargerTerminationCurr(XPOWERS_AXP2101_CHG_ITERM_25MA);

    // Set charge cut-off voltage
    PMU.setChargeTargetVoltage(XPOWERS_AXP2101_CHG_VOL_4V1);

    /*********************************
     * step 4 : Set PMU pwrkey params
    ***********************************/
    // Set the time of pressing the button to turn off
    PMU.setPowerKeyPressOffTime(XPOWERS_POWEROFF_4S);
    uint8_t opt = PMU.getPowerKeyPressOffTime();
    Serial.print("PowerKeyPressOffTime:");
    switch (opt) {
    case XPOWERS_POWEROFF_4S: Serial.println("4 Second");
        break;
    case XPOWERS_POWEROFF_6S: Serial.println("6 Second");
        break;
    case XPOWERS_POWEROFF_8S: Serial.println("8 Second");
        break;
    case XPOWERS_POWEROFF_10S: Serial.println("10 Second");
        break;
    default:
        break;
    }


    // Set the button power-on press time
    PMU.setPowerKeyPressOnTime(XPOWERS_POWERON_128MS);
    opt = PMU.getPowerKeyPressOnTime();
    Serial.print("PowerKeyPressOnTime:");
    switch (opt) {
    case XPOWERS_POWERON_128MS: Serial.println("128 Ms");
        break;
    case XPOWERS_POWERON_512MS: Serial.println("512 Ms");
        break;
    case XPOWERS_POWERON_1S: Serial.println("1 Second");
        break;
    case XPOWERS_POWERON_2S: Serial.println("2 Second");
        break;
    default:
        break;
    }

    // PMU.setChargingLedMode(XPOWERS_CHG_LED_BLINK_4HZ);
}

void irqStatus() {
    if (pmu_flag) {

        pmu_flag = false;

        // Get PMU Interrupt Status Register
        uint32_t status = PMU.getIrqStatus();

        if (PMU.isVbusInsertIrq()) {
            Serial.println("isVbusInsert");
        }
        if (PMU.isVbusRemoveIrq()) {
            Serial.println("isVbusRemove");
        }
        if (PMU.isBatInsertIrq()) {
            Serial.println("isBatInsert");
        }
        if (PMU.isBatRemoveIrq()) {
            Serial.println("isBatRemove");
        }
        if (PMU.isPekeyShortPressIrq()) {
            Serial.println("isPekeyShortPress");
        }
        if (PMU.isPekeyLongPressIrq()) {
            Serial.println("isPekeyLongPress");
        }

        // For more interrupt sources, please check XPowersLib


        // Clear PMU Interrupt Status Register
        PMU.clearIrqStatus();
    }
}

void powerStatus() {
    Serial.print("isCharging:"); Serial.println(PMU.isCharging() ? "YES" : "NO");
    Serial.print("isVbusIn:"); Serial.println(PMU.isVbusIn() ? "YES" : "NO");
    Serial.print("getBattVoltage:"); Serial.print(PMU.getBattVoltage()); Serial.println("mV");
    Serial.print("getVbusVoltage:"); Serial.print(PMU.getVbusVoltage()); Serial.println("mV");
    Serial.print("getSystemVoltage:"); Serial.print(PMU.getSystemVoltage()); Serial.println("mV");

    // The battery percentage may be inaccurate at first use, the PMU will automatically
    // learn the battery curve and will automatically calibrate the battery percentage
    // after a charge and discharge cycle
    if (PMU.isBatteryConnect()) {
        Serial.print("getBatteryPercent:"); Serial.print(PMU.getBatteryPercent()); Serial.println("%");
    }
    Serial.println();
    loopMillis = millis();
}

void pmuCharging() {
    if (pmu_flag) {

        pmu_flag = false;

        // Get PMU Interrupt Status Register
        uint32_t status = PMU.getIrqStatus();

        if (PMU.isVbusInsertIrq()) {
            Serial.println("isVbusInsert");
        }
        if (PMU.isVbusRemoveIrq()) {
            Serial.println("isVbusRemove");
        }
        if (PMU.isBatInsertIrq()) {
            Serial.println("isBatInsert");
        }
        if (PMU.isBatRemoveIrq()) {
            Serial.println("isBatRemove");
        }
        if (PMU.isPekeyShortPressIrq()) {
            Serial.println("isPekeyShortPress");
        }
        if (PMU.isPekeyLongPressIrq()) {
            Serial.println("isPekeyLongPress");
        }

        // For more interrupt sources, please check XPowersLib


        // Clear PMU Interrupt Status Register
        PMU.clearIrqStatus();
    }

    if (millis() - loopMillis > 3000) {
        Serial.print("isCharging:"); Serial.println(PMU.isCharging() ? "YES" : "NO");
        Serial.print("isVbusIn:"); Serial.println(PMU.isVbusIn() ? "YES" : "NO");
        Serial.print("getBattVoltage:"); Serial.print(PMU.getBattVoltage()); Serial.println("mV");
        Serial.print("getVbusVoltage:"); Serial.print(PMU.getVbusVoltage()); Serial.println("mV");
        Serial.print("getSystemVoltage:"); Serial.print(PMU.getSystemVoltage()); Serial.println("mV");

        // The battery percentage may be inaccurate at first use, the PMU will automatically
        // learn the battery curve and will automatically calibrate the battery percentage
        // after a charge and discharge cycle
        if (PMU.isBatteryConnect()) {
            Serial.print("getBatteryPercent:"); Serial.print(PMU.getBatteryPercent()); Serial.println("%");
        }
        Serial.println();
        loopMillis = millis();
    }
}