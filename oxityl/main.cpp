#include "oxityl.h"
#include <iostream>
#include <conio.h>
#define SENSOR_ROWS                     128
#define SENSOR_COLS                     180

uint8_t CMD_read_INT_reg[2] = { 0x44, 0xFF };
uint8_t CMD_SigFrame_collect_CMD[2] = { 0xF3, 0x40 };
uint8_t CMD_SigFrame_receive_CMD[2] = { 0xFC, 0xFF };
uint8_t CMD_pmuConfig[120] = {  0x83, 0x05, 0x84, 0x40, 0x85, 0xF3, 0x87, 0x80, 0x88, 0x95, 0x89, 0x40, 0x8A, 0x00, 0x8B, 0xA2,
								0x8C, 0x14, 0x8D, 0x4A, 0x8E, 0x44, 0x8F, 0x40, 0x90, 0x02, 0x91, 0x04, 0x92, 0x06, 0x93, 0x08,
								0x94, 0x1F, 0x95, 0x1F, 0x96, 0x1F, 0x97, 0x03, 0x98, 0x05, 0x99, 0x07, 0x9A, 0xC3, 0x9B, 0x03,
								0x9C, 0x08, 0x9D, 0x58, 0x9E, 0x20, 0x9F, 0x70, 0xA0, 0x00, 0xA5, 0x10, 0xA6, 0x00, 0xA7, 0x90,
								0xA8, 0x04, 0xA9, 0x02, 0xAA, 0x08, 0xAB, 0x08, 0xAC, 0x08, 0xAD, 0x08, 0xAE, 0x08, 0xAF, 0x08,
								0xB0, 0x02, 0xB1, 0x48, 0xB2, 0x02, 0xB3, 0x08, 0xB4, 0x08, 0xB5, 0x04, 0xB6, 0x04, 0xBA, 0x02,
								0xBB, 0x00, 0xBE, 0x20, 0xBF, 0x22, 0xC0, 0x54, 0xC1, 0x80, 0xC2, 0x00, 0xC3, 0x00, 0xE8, 0x82,
								0xE9, 0x00, 0xEA, 0x34, 0xEB, 0x4F, 0xEC, 0x20 };
uint8_t CMD_configdone[2] = { 0xF6, 0xFF };
uint8_t CMD_wakeup[2] = { 0xF1, 0x20 };
uint8_t CMD_sleep[2] = { 0xF2, 0xFF };
uint8_t CMD_reconfig[4] = { 0x9D, 0x54, 0x9E, 0x22 };
uint8_t CMD_Bigger_LED_Flash[2] = { 0x9c, 0x3E };
uint8_t CMD_restart[2] = { 0xF8, 0xFF };
uint8_t CMD_Wait[2] = { 0xFF, 0xFF };


uint8_t arrINTReg[2] = { 0 };
uint8_t arrImage[SENSOR_ROWS*SENSOR_COLS * 2 + 10] = { 0 };

int main(int argc, char* argv[])
{
	//Initialize
	oxityl *oxi = new oxityl();
	oxi->PowerOff();
	oxi->PowerOn();
	oxi->SetRSTN(false);	//set RST 0
	oxi->SetRSTN(true);	//set RST 1
	::Sleep(5);
	//Sleep- Wake-up
	oxi->Send(CMD_sleep, 2);
	::Sleep(5);
	oxi->Send(CMD_wakeup, 2);
	::Sleep(5);

	oxi->Read(CMD_read_INT_reg, 2, arrINTReg, 2);
	::Sleep(5);
	//Configure registers
	//uint32_t tempSize = sizeof(CMD_pmuConfig);
	oxi->ConfigPMU(CMD_pmuConfig, sizeof(CMD_pmuConfig));	//set all PMU registers
	oxi->Send(CMD_configdone, 2);	//send config complete CMD
	oxi->Send(&(CMD_reconfig[0]), 2);
	oxi->Send(&(CMD_reconfig[2]), 2);
	oxi->Send(CMD_Bigger_LED_Flash, 2);	//Set LED flashing light intensity max
	oxi->Read(CMD_read_INT_reg, 2, arrINTReg, 2);
	::Sleep(5);



	//Collect One Frame Image
	//oxi->Send(CMD_restart, 2);
	//oxi->Read(CMD_read_INT_reg, 2, arrINTReg, 2);
	oxi->Send(CMD_SigFrame_collect_CMD, 2);
	::Sleep(100);
	while ((0x40 != arrINTReg[0]) || (0x69 != arrINTReg[1]))
	{
		oxi->Read(CMD_read_INT_reg, 2, arrINTReg, 2);
		::Sleep(5);
	}
	
	
	 //Receive One Frame Image
	oxi->GetImage_all(arrImage, CMD_SigFrame_receive_CMD, sizeof(arrImage));
	//oxi->GetImage(arrImage, CMD_SigFrame_receive_CMD, sizeof(arrImage));

	 //Restart Before Collect Frame
	oxi->Send(CMD_restart, 2);

	 //Power Off
	oxi->PowerOff();




	//do{
	//	std::cout << "Main Menu:" << std::endl;
	//	std::cout << "(1) : Initialize" << std::endl;
	//	std::cout << "(2) : Sleep" << std::endl;
	//	std::cout << "(3) : WakeUp" << std::endl;
	//	std::cout << "(4) : Configure all registers" << std::endl;
	//	std::cout << "(5) : Collect One Frame" << std::endl;
	//	std::cout << "(6) : Receive One Frame" << std::endl;
	//	std::cout << "(7) : Power Off" << std::endl;



	//	char key = _getch();
	//	if ('1' == key)
	//	{
	//		oxi->PowerOn();
	//		oxi->SetRSTN(false);	//set RST 0
	//		oxi->SetRSTN(true);	//set RST 1
	//		oxi->Read(arrINTReg, CMD_read_INT_reg, 2);
	//	}
	//	else if ('2' == key)
	//	{
	//		oxi->Send(CMD_sleep, sizeof(CMD_sleep));
	//		oxi->Read(arrINTReg, CMD_read_INT_reg, 2);
	//	}
	//	else if ('3' == key)
	//	{
	//		oxi->Send(CMD_wakeup, sizeof(CMD_wakeup));
	//		oxi->Read(arrINTReg, CMD_read_INT_reg, 2);
	//	}
	//	else if ('4' == key)
	//	{
	//		oxi->Send(CMD_pmuConfig, sizeof(CMD_pmuConfig));	//set all PMU registers
	//		oxi->Send(CMD_configdone, sizeof(CMD_configdone));	//send config complete CMD
	//		oxi->Send(CMD_reconfig, sizeof(CMD_reconfig));
	//		oxi->Send(CMD_Bigger_LED_Flash, sizeof(CMD_Bigger_LED_Flash));	//Set LED flashing light intensity max
	//		oxi->Read(arrINTReg, CMD_read_INT_reg, 2);
	//	}
	//	else if ('5' == key)
	//	{
	//		oxi->Send(CMD_restart, sizeof(CMD_restart));
	//		oxi->Read(arrINTReg, CMD_read_INT_reg, 2);
	//		oxi->Send(CMD_SigFrame_collect_CMD, sizeof(CMD_SigFrame_collect_CMD));
	//		oxi->Read(arrINTReg, CMD_read_INT_reg, 2);
	//	}
	//	else if ('6' == key)
	//	{
	//		oxi->Read(arrImage, CMD_SigFrame_receive_CMD, sizeof(arrImage));
	//		oxi->Read(arrINTReg, CMD_read_INT_reg, 2);
	//	}
	//	else if ('7' == key)
	//	{
	//		oxi->PowerOff();
	//	}
	//	else
	//	{
	//		return 0;
	//	}
	//} while (true);


	//oxi->Send(CMD_wakeup, 2);	//send wake up Cmd

	//
	//oxi->Send(CMD_Bigger_LED_Flash, sizeof(CMD_Bigger_LED_Flash));	//Set LED flashing light intensity max
	//oxi->Read(arrINTReg, CMD_read_INT_reg, 2);	//check INT reg if 0x8002(READY FOR CMD)

	//oxi->Send(CMD_restart, 2);

	//oxi->Send(CMD_SigFrame_collect_CMD, 2);	// collect 1 frame store in PMU RAM

	//oxi->Read(arrINTReg, CMD_read_INT_reg, 2);	//check INT reg if 0x4069(frame ready)

	//oxi->Read(arrImage, CMD_SigFrame_receive_CMD, sizeof(arrImage));	//check INT reg if 0x4069(frame ready)
	//oxi->Send(CMD_restart, 2);

	/*oxi->PowerOff();*/

	delete oxi;
	oxi = NULL;
}