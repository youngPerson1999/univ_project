int csd_main()
{
	unsigned *SWI = (unsigned *) 0x41210000;	//switch 주소 넣어주기
	int dur;
	//0b10000000->7스위치 on ~ 0b00000000->모든 스위치 off
	if(*SWI >= 0b10000000)
		dur = 0x02000000;
	else if(*SWI >= 0b01000000)
		dur = 0x04000000;
	else if (*SWI >= 0b00100000)
		dur = 0x06000000;
	else if(*SWI >= 0b00010000)
		dur = 0x08000000;
	else if(*SWI >= 0b00001000)
		dur = 0x0a000000;
	else if(*SWI >= 0b00000100)
		dur = 0x0c000000;
	else if(*SWI >= 0b00000010)
		dur = 0x0e000000;
	else if(*SWI >= 0b00000001)
		dur = 0x10000000;
	else
		dur = 0x14000000;
	return dur;
}
