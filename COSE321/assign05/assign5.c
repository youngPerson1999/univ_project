//#pragma GCC target ("thumb")
#pragma GCC target ("arm")


void thumb_C_test(int a)
{
	int indata[32] = {	2, 0, -7, -1, 3, 8, -4, 10,
						-9, -16, 15, 13, 1, 4, -3, 14,
						-8, -10, -15, 6, -13, -5, 9, 12,
						-11, -14, -6, 11, 5, 7, -2, -12 };
	int outdata[32];

	for(int i = 0 ; i < 32 ; i++)
		outdata[i] = indata[i];
	for(int i = 0 ; i < 31 ; i++){
		for(int j = 0; j < 31 - i ; j++){
			if(outdata[j] > outdata[j + 1]){
				int temp = outdata[j];
				outdata[j] = outdata[j + 1];
				outdata[j + 1] = temp;
			}
		}
	}

}




