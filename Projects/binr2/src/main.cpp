#include "binr2.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argn, char *args[])
{
	int i = 0;
	cout << "Hello, Binr2" << endl;
	
	/*  // Тест распаковщика от файла 
	    if(argn > 1){
		ifstream file(args[1]);
		if(file.is_open()){
			Binr2 b2Processor;
			cout << "File open OK " << endl;
			while(!file.eof()){
				char byte = 0;
				file.read(&byte, 1);
				if(b2Processor.decode(byte)){
					cout << hex << "Get Binr2 pkg: " << int(b2Processor.getDecodePkgId()) << endl;
					break;
				}
				Binr2::DecodeErrors b2Err = b2Processor.getDecodeError();
				if(b2Err != Binr2::DecodeError_NoError){
					
					cout << "Get Binr2 err id: " << hex << b2Err << endl;
					switch(b2Err){
						case Binr2::DecoderError_InternalBufferError:
							cout << dec << "Internal buffer err. Needs " << b2Processor.getDecodeDataSizeField() << " bytes "<< endl;
						break;
						default:;
					}
					b2Processor.resetDecodeError();
					break;
				}
				i++;
			};
			cout << dec << "Total file size process " << i << " bytes" << endl;
		} else {
			cout << "File open error" << endl;
		}
	} else {
		cout << "No input file" << endl;
	}*/
	/* //Проверка упаковщика
	Binr2 b2Processor;
	
	b2Processor.setEncodePkgId(0x80);
	if(b2Processor.getEncodeError() != Binr2::EncodeError_NoError){
		cout << "Encode 1 Binr2 error" << hex << b2Processor.getEncodeError() << endl;
		b2Processor.resetEncodeError();
	}
	unsigned char  data[2]  = {0x01, 0x00};
	unsigned char  out[128];
	unsigned short outLen = 0;
	b2Processor.setEncodeData(data, sizeof(data));
	if(b2Processor.getEncodeError() != Binr2::EncodeError_NoError){
		cout << "Encode 2 Binr2 error" << hex << b2Processor.getEncodeError() << endl;
		b2Processor.resetEncodeError();
	}
	outLen = b2Processor.encode(out, sizeof(out));
	
	if(b2Processor.getEncodeError() != Binr2::EncodeError_NoError){
		cout << "Encode 3 Binr2 error " << hex << b2Processor.getEncodeError() << endl;
		b2Processor.resetEncodeError();
	}
	
	cout << "Data out = " << outLen << endl;
	for(i = 0; i < outLen; i++){
		cout << hex << int(out[i]) << " ";
	}
	cout << endl;
	
	for(int k = 0; k < outLen; k++){
		if(b2Processor.decode(out[k])){
			cout << "Decode = " << hex << int(b2Processor.getDecodePkgId()) << endl;
		}
	}
	*/
	cout << "Bye-bye, Binr2" << endl;
	
	return 0;
}