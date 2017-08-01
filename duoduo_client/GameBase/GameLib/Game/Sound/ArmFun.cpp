
/* ------------------------------------------------------------------
* Copyright (C) 2009 Martin Storsjo
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
* express or implied.
* See the License for the specific language governing permissions
* and limitations under the License.
* -------------------------------------------------------------------
*/
#include "cocos2d.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "amr_nb/interf_enc.h"
#include "amr_nb/wavreader.h"
#include "amr_nb/wavwriter.h"
#include "amr_nb/interf_dec.h"

namespace ArmFun
{
	int WavToArm(std::string  pchWAVEFilename, std::string pchAMRFileName)
	{
		pchWAVEFilename =  cocos2d::CCFileUtils::getInstance()->getSuitableFOpen(pchWAVEFilename);
		pchAMRFileName =  cocos2d::CCFileUtils::getInstance()->getSuitableFOpen(pchAMRFileName);
		enum Mode mode = MR59;
		int dtx = 0;
		const char *infile, *outfile;
		FILE *out;
		void *wav, *amr;
		int format, sampleRate, channels, bitsPerSample;
		int inputSize;
		uint8_t* inputBuf;
		infile = pchWAVEFilename.c_str();
		outfile = pchAMRFileName.c_str();

		wav = wav_read_open(infile);
		if (!wav) {
			cocos2d::log("Unable to open wav file %s\n","");
			cocos2d::log("%s",infile);
			return 1;
		}
		if (!wav_get_header(wav, &format, &channels, &sampleRate, &bitsPerSample, NULL)) {
			cocos2d::log("Bad wav file %s\n","");
			return 1;
		}
		if (format != 1) {
			cocos2d::log("Unsupported WAV format %s\n","");
			return 1;
		}
		if (bitsPerSample != 16) {
			cocos2d::log("Unsupported WAV sample depth %s\n","");
			return 1;
		}
		if (channels != 1)
			cocos2d::log("Warning, only compressing one audio channel%s\n","");
		if (sampleRate != 8000)
			cocos2d::log("Warning, AMR-NB uses 8000 Hz sample rate (WAV file has %d Hz)%s\n","");
		int iRate = sampleRate/8000;
		if (iRate <= 0)
		{
			iRate = 1;
		}

		inputSize = channels*2*160*iRate;
		inputBuf = (uint8_t*) malloc(inputSize);

		amr = Encoder_Interface_init(dtx);
		out = fopen(outfile, "wb+");
		if (!out) {
			perror(outfile);
			cocos2d::log("Warning, if (!out) \n","");
			return 1;
		}

		fwrite("#!AMR\n", 1, 6, out);
		while (1) {
			short buf[160];
			uint8_t outbuf[500];
			int read, i, n;
			read = wav_read_data(wav, inputBuf, inputSize);
			read /= channels;
			read /= 2;
			read /= iRate;
			if (read < 160)
				break;
			for (i = 0; i < 160; i++) {
				const uint8_t* in = &inputBuf[2*channels*i];
				buf[i] = in[0] | (in[1] << 8);
			}
			n = Encoder_Interface_Encode(amr, mode, buf, outbuf, 0);
			fwrite(outbuf, 1, n, out);
		}
		free(inputBuf);
		fclose(out);
		Encoder_Interface_exit(amr);
		wav_read_close(wav);
		return 1;
	}

	int ArmToWav(std::string pchAMRFileName,std::string pchWAVEFilename)
	{
		pchWAVEFilename =  cocos2d::CCFileUtils::getInstance()->getSuitableFOpen(pchWAVEFilename);
		pchAMRFileName =  cocos2d::CCFileUtils::getInstance()->getSuitableFOpen(pchAMRFileName);
		FILE* in;
		char header[6];
		int n;
		void *wav, *amr;
		in = fopen(pchAMRFileName.c_str(), "rb");
		if (!in) {
			cocos2d::log("ArmToWav Unable to open AMR file %s\n","");
			return 1;
		}
		n = fread(header, 1, 6, in);
		if (n != 6 || ::memcmp(header, "#!AMR\n", 6)) {
			cocos2d::log("ArmToWav Bad header\n","");
			return 1;
		}

		wav = wav_write_open(pchWAVEFilename.c_str(), 8000, 16, 1);
		if (!wav) {
			cocos2d::log("ArmToWav Unable to open WAV file %s\n","");
			return 1;
		}

		const int sizes[] = { 12, 13, 15, 17, 19, 20, 26, 31, 5, 6, 5, 5, 0, 0, 0, 0 };
		amr = Decoder_Interface_init();
		while (1) {
			uint8_t buffer[500], littleendian[320], *ptr;
			int size, i;
			int16_t outbuffer[160];
			/* Read the mode byte */
			n = fread(buffer, 1, 1, in);
			if (n <= 0)
				break;
			/* Find the packet size */
			size = sizes[(buffer[0] >> 3) & 0x0f];
			n = fread(buffer + 1, 1, size, in);
			if (n != size)
				break;

			/* Decode the packet */
			Decoder_Interface_Decode(amr, buffer, outbuffer, 0);

			/* Convert to little endian and write to wav */
			ptr = littleendian;
			for (i = 0; i < 160; i++) {
				uint8_t temp1,temp2;
				temp1 = ((outbuffer[i] >> 0) & 0xff);
				temp2 = ((outbuffer[i] >> 8) & 0xff);
				temp1 = temp1*(temp1/256+1.0f);
				temp2 = temp2*(temp2/256+1.0f);
				*ptr++ = temp1;
				*ptr++ = temp2;
			}
			wav_write_data(wav, littleendian, 320);
		}
		fclose(in);
		Decoder_Interface_exit(amr);
		wav_write_close(wav);
		return 1;
	}
}