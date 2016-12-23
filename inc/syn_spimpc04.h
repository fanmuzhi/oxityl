#pragma once

#include "syn_bridge.h"

/* -------------------------------------------------------------------------- */
/* General macros */

/* DRDY is on B4: */
#define SPIMPC04_GPIO_DRDY_PORT      1
#define SPIMPC04_GPIO_DRDY_B         4

/* SLEEP_N is on H4: */
#define SPIMPC04_GPIO_SLEEP_N_PORT   7
#define SPIMPC04_GPIO_SLEEP_N_B      4

/* Switch is on J4: */
#define SPIMPC04_GPIO_SWITCH_PORT	 8
#define SPIMPC04_GPIO_SWITCH_B       4

/* OSC trim is on G7: */
#define SPIMPC04_GPIO_OSCTRIM_PORT   6
#define SPIMPC04_GPIO_OSCTRIM_B      7

/*
* What is the offset of the first byte past the given member of a struct/union?
*/
#define OFFSETPAST(t, m)    (((unsigned int)&(((t *) 0)->m))               \
	+ sizeof(((t *)0)->m))

/* What is the offset of the given member of a structure/union? */
#define OFFSETOF(t, m)  ((unsigned int) &(((t *) 0)->m))

/*  Return a pointer to an object given a pointer to something within it */
#define FROMWITHIN(objp, type, memb)                                        \
	((type *)(((unsigned char *)(objp))                                   \
	- ((unsigned int)&(((type *)0)->memb))))

#define NELEM(x)        (sizeof((x)) / sizeof((x)[0]))

/*
* Confusing.  These maxima are defined in terms from the MPC04's
*  perspective -- 'RX' is the maximum length of data the host can send,
*  'TX' is the maximum length of data that the host can receive.
*/
#define SPIMPC04_MAX_RX_MSG_SIZE        1024
#define SPIMPC04_MAX_TX_MSG_SIZE        510

/* SPI baud rate. Currently it is temporary set to 4MHz as there is some issue
in MPC04 device with higher baud rate. We will set it to 9600MHz after
confirmation from the HW team that the MPC04 can work with this baud rate. */
#define SPIMPC04_SPEED              4000
/* SPI port ID - UUT port=0, aux port=1 */
#define SPIMPC04_MODE               0
#define SPIMPC04_PORTID             0
#define SPIMPC04_CHIPSELECT         0x0008
#define SPIMPC04_INTER_BYTE_DELAY   0

#define SPIMPC04_TIMEOUT			2000

/*
* Endian switchers.  We assume that dactyl is running on a little-endian
*  machine and the MPC04 is big-endian.
*/
#define SPIMPC04_SWAP16(val)      ((((val) & 0x00ff) << 8)               \
	| (((val) >> 8) & 0x00ff))
#define SPIMPC04_SWAP32(val)      ((((val) & 0x000000ff) << 24)          \
	| (((val)& 0x0000ff00) << 8)         \
	| (((val)& 0x00ff0000) >> 8)         \
	| (((val)& 0xff000000) >> 24))


#define SPIMPC04_MPCTOHOST8(valp)     (*((const uint8_t *) (valp)))
#define SPIMPC04_MPCTOHOST16(valp)                                       \
	SPIMPC04_SWAP16(*((const uint16_t *)(valp)))
#define SPIMPC04_MPCTOHOST32(valp)                                       \
	SPIMPC04_SWAP32(*((const uint32_t *)(valp)))

#define SPIMPC04_HOSTTOMPC8(destp, val) (*((uint8_t *) (destp)) = (val))
#define SPIMPC04_HOSTTOMPC16(destp, val)                                 \
	(*((uint16_t *)(destp)) = SPIMPC04_SWAP16(val))
#define SPIMPC04_HOSTTOMPC32(destp, val)                                 \
	(*((uint32_t *)(destp)) = SPIMPC04_SWAP32(val))


typedef struct {
	unsigned char    cmd[4];
	uint16_t         transactionId;
	uint16_t         msgSize;
	uint16_t         error;
	uint16_t         fmtVersion;        /* always 0 */
	uint16_t         size;              /* always 0 */
} spimpc04_hdr_t;

/*
* Identity
*/
typedef struct {
	spimpc04_hdr_t   hdr;        /* 'Idnt' */
} spimpc04_cmd_identity_t;

typedef struct {
	spimpc04_hdr_t      hdr;        /* 'Idnt' */

	uint16_t            identifier; /* Identifies the diagnostics controller. */
	uint32_t            bootVersion; /* b3=major, b2=minor, b1=micro, b0=nano */
	uint32_t            applVersion; /* b3=major, b2=minor, b1=micro, b0=nano */
} spimpc04_reply_identity_t;

/*
* SetVoltages
*/
typedef struct {
	spimpc04_hdr_t      hdr;        /* 'Volt' */

	uint16_t            vdd_mv;
	uint16_t            vIo_mv;
	uint16_t            vLed_mv;
	uint16_t            vTx_mv;
} spimpc04_cmd_setvoltages_t;

typedef struct {
	spimpc04_hdr_t      hdr;        /* 'Volt' */
} spimpc04_reply_setvoltages_t;

/*
* SetPortToSpi
*/
typedef struct {
	spimpc04_hdr_t      hdr;        /* 'SSpi' */

	uint16_t            port;       /* 0=main port, 1=aux port. */
	uint16_t            slaveSelect; /* Slave select identifier. */
	uint16_t            speed_kHz;
	uint16_t            spiMode;    /* b1=CPOL, b0=CPHA. */
	uint16_t            interByteDelay_us; /* useconds */
} spimpc04_cmd_setporttospi_t;

typedef struct {
	spimpc04_hdr_t      hdr;        /* 'SSpi' */
} spimpc04_reply_setporttospi_t;

/*
* GpioModDirSet
*/

typedef struct {
	spimpc04_hdr_t      hdr;       /* 'GDir' */

	uint16_t            portId;    /* Stellaris port id (0=A, 1=B...) */
	uint32_t            mskPins;   /* pins involved in this operation */
	uint16_t            direction; /* 0=input, 1=output, 2=hardware */
} spimpc04_cmd_gpiomoddirset_t;

/*
* GpioSetPinType
*/

typedef struct {
	spimpc04_hdr_t      hdr;       /* 'PinT' */

	uint16_t            portId;    /* Stellaris port id (0=A, 1=B...) */
	uint32_t            mskPins;   /* pins involved in this operation */
	uint16_t            pinType;   /* SPIMPC04_GPIO_TYPE_PIN_DIG_INPUT,
								   * SPIMPC04_GPIO_TYPE_PIN_DIG_OUTPUT, etc. */
} spimpc04_cmd_gpiosetpintype_t;

/*
* GpioPinRead
*/
typedef struct {
	spimpc04_hdr_t      hdr;        /* 'PinR' */

	uint16_t            portId;    /* Stellaris port id (0=A, 1=B...) */
	uint32_t            mskPins;   /* pins involved in this operation */
} spimpc04_cmd_gpiopinread_t;

typedef struct {
	spimpc04_hdr_t      hdr;        /* 'PinR' */

	uint32_t            mskValues;  /* Value of pins. */
} spimpc04_reply_gpiopinread_t;

/*
* GpioPinWrite
*/
typedef struct {
	spimpc04_hdr_t      hdr;        /* 'PinW' */

	uint16_t            portId;     /* Stellaris port id (0=A, 1=B...) */
	uint32_t            mskPins;    /* pins involved in this operation */
	uint32_t            mskValues;  /* Values of pins */
} spimpc04_cmd_gpiopinwrite_t;

/*
* ReadPortData
*/
typedef struct {
	spimpc04_hdr_t      hdr;        /* 'RPrt' */

	uint16_t            portNum;    /* 0=main port, 1=aux port. */
	uint16_t            numBytes;   /* Number of bytes to read. */
	uint16_t            assertSS;   /* True=assert SS before write. */
	uint16_t            deassertSS; /* True=deassert SS after write. */
} spimpc04_cmd_readportdata_t;

typedef struct {
	spimpc04_hdr_t      hdr;        /* 'RPrt' */

	uint16_t            numRead;    /* Number of bytes read. */
	/* Followed by payload data */
} spimpc04_reply_readportdata_t;

/*
* WritePortData
*/
typedef struct {
	spimpc04_hdr_t      hdr;        /* 'WPrt' */

	uint16_t            portNum;    /* 0=main port, 1=aux port. */
	uint16_t            numBytes;   /* Number of bytes to read. */
	uint16_t            assertSS;   /* True=assert SS before write. */
	uint16_t            deassertSS; /* True=deassert SS after write. */
	/* Followed by payload data */
} spimpc04_cmd_writeportdata_t;

typedef struct {
	spimpc04_hdr_t      hdr;        /* 'WPrt' */

	uint16_t            numWritten; /* Number of bytes read. */
} spimpc04_reply_writeportdata_t;

/*
 * GetCurrentSenseValue
 */
typedef struct {
	spimpc04_hdr_t      hdr;       /* 'GCur' */

	uint16_t            gain; 
	uint16_t            overSample;
} spimpc04_cmd_getcursenseval_t;

typedef struct {
	spimpc04_hdr_t      hdr;       /* 'GCur' */

	uint32_t            values[4];
} spimpc04_reply_getcursenseval_t;

typedef struct
{
	HANDLE					deviceHandle;
	WINUSB_INTERFACE_HANDLE usbHandle;

	UCHAR deviceSpeed;
	UCHAR bulkInPipe;
	UCHAR bulkOutPipe;
	UCHAR interruptPipe;
}spimpc04_id;

class TransactionId
{
public:
	static uint16_t GetId()
	{
		uint16_t current = m_id;
		if ((m_id + 1) > 0xFFFF)
		{
			m_id = 0;
		}
		else
		{
			m_id++;
		}
		return current;
	}

private:

	static uint16_t m_id;
};

class syn_spimpc04 : public syn_bridge
{
public:
	syn_spimpc04(string serialnumber);
	virtual ~syn_spimpc04();

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

	uint32_t GetCurrentValues(uint32_t arrValues[2], bool lowGain = true, uint32_t timeout = 2000);

	uint32_t Write(const uint8_t *datap, unsigned int nbytes, BOOL deassert_mcs_after);

	uint32_t Read(uint8_t *datap, unsigned int nbytes, BOOL deassert_mcs_after);

protected:

	spimpc04_id _spimpc04_id;

private:

	uint32_t setportspi(unsigned int portid, unsigned int chipsel, unsigned int speed, unsigned int spimode, unsigned int interbytedelay, unsigned int timeout);

	uint32_t getCurrentSenseValues(uint16_t gain, uint16_t oversample, uint32_t arrValues[4], uint32_t timeout);

	uint32_t readgpios(uint16_t portid, uint32_t mskpins, uint32_t *gpiovalp, uint32_t timeout);

	uint32_t writegpios(uint16_t portid, uint32_t mskpins, uint32_t mskvalues, uint32_t timeout);

	uint32_t gpiopinread(uint16_t portid, uint32_t mskpins, uint32_t *mskvalues, uint32_t timeout);

	uint32_t gpiopinwrite(uint16_t portid, uint32_t mskpins, uint32_t mskvalues, uint32_t timeout);

	void hdrfill(spimpc04_hdr_t *hdrp, char cmd[4], unsigned int transactionId, unsigned int msgSize);

	uint32_t gpiosetpintype(uint16_t portid, uint32_t mskpins, uint16_t pintype, uint32_t timeout);

	uint32_t gpiomoddirset(uint16_t portid, uint32_t mskpins, uint32_t direction, uint32_t timeout);

	uint32_t execute(const void *cmdbufp, unsigned int cmdbuflen, void *replybufp, unsigned int replybuflen, unsigned int *replylenp, DWORD timeout);

	uint32_t deassertmcs();

	uint32_t drdycheck(BOOL *drdyp, uint32_t timeout);

	uint32_t sleepsigset(BOOL sleepval, uint32_t timeout);
};

