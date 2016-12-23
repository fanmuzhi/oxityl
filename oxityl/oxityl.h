#include <stdint.h>
#include <vector>
#include "syn_devicemanager.h"
#include "syn_bridge.h"

using namespace std;

class oxityl
{
	devicetype type;
	vector<string> serialnumberList;
	syn_bridge *pBridge = NULL;
	uint32_t clockRate = 6;

	public:
		oxityl();
		
		uint32_t oxityl::PowerOn();
		uint32_t oxityl::PowerOff();
		uint32_t oxityl::SetRSTN(bool RST_Val);
		uint32_t oxityl::Send(uint8_t *arrCMD, uint32_t size, bool deassert_mcs = true);
		//uint32_t oxityl::Read(uint8_t *arrReply, uint32_t size);
		//uint32_t oxityl::Read(uint8_t *arrReply, uint8_t *arrCMD, uint32_t size);
		uint32_t oxityl::Read(uint8_t *arrCMD, uint32_t size, uint8_t *arrReply, uint32_t replySize, bool wr_deassert_mcs = false);
		uint32_t oxityl::ConfigPMU(uint8_t *arrCMD, uint32_t size);
		uint32_t oxityl::GetImage(uint8_t *arrReply, uint8_t *ReadCmd, uint32_t size);
		uint32_t oxityl::GetImage_all(uint8_t *arrReply, uint8_t *ReadCmd, uint32_t size);
		virtual ~oxityl();
		//uint32_t INT_reg_is(uint16_t Is_value)
protected:
	syn_bridge *_pSynBridge;
};

