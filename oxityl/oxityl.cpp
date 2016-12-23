#include <stdio.h>
#include <iostream>  

#include "oxityl.h"
#include "syn_devicemanager.h"
#include "syn_bridge.h"
using namespace std;

oxityl::oxityl()
{
	syn_devicemanager *pDeviceManager = NULL;
	pDeviceManager = new syn_devicemanager();

	uint32_t rc = pDeviceManager->Connect(spi_m5);
	if (rc == 0)
	{
		rc = pDeviceManager->GetSerialNumberList(serialnumberList);
		type = spi_m5;
		clockRate = 6;
	}
	else
	{
		rc = pDeviceManager->Connect(spi_mpc04);
		if (rc == 0)
		{
			rc = pDeviceManager->GetSerialNumberList(serialnumberList);
			type = spi_mpc04;
			clockRate = 6;
		}
	}
	delete pDeviceManager;
	pDeviceManager = NULL;
	cout << "serial number is " << serialnumberList[0] << endl;
	uint32_t status = syn_bridge::CreateDeviceInstance(serialnumberList[0], type, pBridge);
	if (0 != status&&NULL == pBridge)
		throw exception("Create syn_bridge failed");
		
	status = pBridge->SetPortSPI(clockRate, 2000);
	if (0 != status)
		throw exception("SetPortSPI failed");


}


uint32_t oxityl::PowerOn()
{
	uint32_t rc;
	rc = pBridge->SetVoltages(3300, 1800);
	::Sleep(5);
	return rc;
}

uint32_t oxityl::PowerOff()
{
	uint32_t rc;
	rc = pBridge->SetVoltages(0, 0);
	return rc;
}

uint32_t oxityl::SetRSTN(bool RST_Val)
{
	uint32_t rc; 
	rc = pBridge->GPIO_SetDRDY(RST_Val);
	//bool drdy;
	//return pBridge->GPIO_CheckDRDY(&drdy, 2000);
	return rc;
}


uint32_t oxityl::Send(uint8_t *arrCMD, uint32_t size, bool deassert_mcs)
{
	uint32_t rc(0);
	rc = pBridge->Write(&(arrCMD[0]), size, deassert_mcs);
	//::Sleep(5);
	return rc;
}


uint32_t oxityl::Read(uint8_t *arrCMD, uint32_t size, uint8_t *arrReply, uint32_t replySize, bool wr_deassert_mcs)
{
	uint32_t rc = 0;
	//uint8_t CMD_Wait[2] = { 0xFF, 0xFF };
	rc = pBridge->Write(&(arrCMD[0]), size, wr_deassert_mcs);
	if (0 == rc)
	{
		//rc = pBridge->Write(&(CMD_Wait[0]), 0, true);		
		rc = pBridge->Read(&(arrReply[0]), replySize, true);

	}
	return rc;
}

//uint32_t oxityl::Read(uint8_t *arrReply, uint8_t *ReadCmd, uint32_t size)
//{
//	uint32_t rc = 0;
//	uint32_t replyLength = size;
//	uint8_t *arrReplyBuf = new uint8_t[replyLength];
//	rc = pBridge->Write(ReadCmd, sizeof(ReadCmd), true);
//	if (0 == rc)
//	{
//		rc = pBridge->Read(arrReplyBuf, replyLength, true);
//	}
//	//
//	if (0 == rc)
//	{
//		memcpy(arrReply, arrReplyBuf, replyLength);
//	}
//	delete[] arrReplyBuf;
//	arrReplyBuf = NULL;
//	return rc;
//}

uint32_t oxityl::ConfigPMU(uint8_t *pmuRegConfig, uint32_t size)
{
	uint32_t rc = 0;
	if (0 != size % 2)
		return 1;
	for (int i = 0; i < size;i=i+2)
	{
		if (0 == rc)
		{
			rc = Send(&(pmuRegConfig[i]), 2);
		}
	}
	return 0;
}

uint32_t oxityl::GetImage_all(uint8_t *arrReply, uint8_t *ReadCmd, uint32_t size)
{
	uint32_t rc = 0;
	uint8_t CMD_Wait[2] = { 0xFF, 0xFF };
	if (0 != size % 2)
		return 1;
	//rc = Send(ReadCmd, 2);
	rc = pBridge->Write(ReadCmd, 2, false);
	//rc = Send(CMD_Wait, 2);
	//rc = Send(CMD_Wait, 2);
	uint8_t arr[2] = { 0xff, 0xff };
	if (0 == rc)
	{

		//rc = pBridge->Write(CMD_Wait, 2, false);
		rc = pBridge->Read(&(arrReply[0]), size, true);
		//this->Read(arr,0,&(arrReply[i]), 2);
	}
	return 0;
}


uint32_t oxityl::GetImage(uint8_t *arrReply, uint8_t *ReadCmd, uint32_t size)
{
	uint32_t rc = 0;
	uint8_t CMD_Wait[2] = { 0xFF, 0xFF };
	if (0 != size % 2)
		return 1;
	//rc = Send(ReadCmd, 2);
	rc = pBridge->Write(ReadCmd, 2, true);
	rc = Send(CMD_Wait, 2);
	rc = Send(CMD_Wait, 2);
	

	uint8_t arr[2] = { 0xff, 0xff };
	if (0 == rc)
	{
		for (int i = 0; i < size; i = i+2)
		{
			rc = pBridge->Write(CMD_Wait, 2, false);
			rc = pBridge->Read(&(arrReply[i]), 2, true);
			//this->Read(arr,0,&(arrReply[i]), 2);
		}
	}
	return 0;
}

oxityl::~oxityl()
{
	delete pBridge;
	pBridge = NULL;
	serialnumberList.clear();
}

