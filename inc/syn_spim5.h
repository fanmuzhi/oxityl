#pragma once

#include "syn_bridge.h"

//ftdi
extern "C"
{
#include "ftd2xx.h"
}
#include "syna_mpsse.h"

typedef struct
{
	/*FT_HANDLE*/
	HANDLE					deviceHandleA;//spi
	HANDLE					deviceHandleB;//i2c

}spim5_id;

class syn_spim5 : public syn_bridge
{
public:
	syn_spim5(string serialnumber);
	virtual ~syn_spim5();

	uint32_t Init();

	uint32_t SetPortSPI(uint32_t clockRate, unsigned int timeout);

	uint32_t GetIdentity(uint32_t *identifierp, uint32_t *boot_versionp, uint32_t *appl_versionp, unsigned int timeout);

	uint32_t SetVoltages(unsigned int vcc, unsigned int spivcc, unsigned int timeout = 2000, unsigned int sleeptime = 20);

	uint32_t GPIO_CheckDRDY(bool *drdyp, uint32_t timeout);

	uint32_t GPIO_SetDRDY(bool drdyval, uint32_t timeout);

	uint32_t GPIO_CheckSleepN(bool *sleepval, uint32_t timeout);

	uint32_t GPIO_SetSleepN(bool sleepval, uint32_t timeout);

	uint32_t GPIO_CheckSwitch(bool *switchval, uint32_t timeout);

	uint32_t GPIO_SetSwitch(bool switchval, uint32_t timeout);

	uint32_t GPIO_CheckOSCTrim(bool *trimchval, uint32_t timeout);

	uint32_t GPIO_SetOSCTrim(bool trimval, uint32_t timeout);

	uint32_t GetCurrentValues(uint32_t arrValues[2], bool lowGain = true, uint32_t timeout = 2000); //arrValues - [0]:1.8,[1]:3.3

	uint32_t Write(const uint8_t *datap, unsigned int nbytes, BOOL deassert_mcs_after);

	uint32_t Read(uint8_t *datap, unsigned int nbytes, BOOL deassert_mcs_after);

protected:

	spim5_id _spim5_id;

private:

	uint8_t calcVolts(uint32_t voltage);
	uint8_t calcSPIVolts(uint32_t voltage);
	uint32_t getChannelCurrent(uint32_t I2CAddress, uint32_t &value);
	uint32_t deassertmcs();
};

