/*
Copyright (c) 2018 Teijo Hakala

See the file LICENSE.md for copying permission.
*/

#include "WavExporter.h"
#include <iostream>
#include <QDir>
#include <QDataStream>

using namespace std;

void WavExporter::Export(QByteArray *bufferI, QByteArray *bufferQ, unsigned int sampleRate, unsigned int bitsPerSample)
{
	// Write wav pcm header
	unsigned int length = bufferI->length();

	unsigned int chunkSize = 36 + length;
	unsigned int formatSize = 16;
	unsigned short dataFormat = 1;

	short channels = 2;

	unsigned int bytesPerSec = bitsPerSample * sampleRate * channels / 8;
	short frameSize = bitsPerSample * channels / 8;

	QByteArray headerBuffer;

	headerBuffer.append("RIFF", 4);

	headerBuffer.append(chunkSize&0xFF).append((chunkSize>>8)&0xFF);
	headerBuffer.append((chunkSize>>16)&0xFF).append((chunkSize>>24)&0xFF);

	headerBuffer.append("WAVE", 4);

	headerBuffer.append("fmt ", 4);

	headerBuffer.append(formatSize&0xFF).append((formatSize>>8)&0xFF);
	headerBuffer.append((formatSize>>16)&0xFF).append((formatSize>>24)&0xFF);

	headerBuffer.append(dataFormat&0xFF).append((dataFormat>>8)&0xFF);

	headerBuffer.append(channels&0xFF).append((channels>>8)&0xFF);

	headerBuffer.append(sampleRate&0xFF).append((sampleRate>>8)&0xFF);
	headerBuffer.append((sampleRate>>16)&0xFF).append((sampleRate>>24)&0xFF);

	headerBuffer.append(bytesPerSec&0xFF).append((bytesPerSec>>8)&0xFF);
	headerBuffer.append((bytesPerSec>>16)&0xFF).append((bytesPerSec>>24)&0xFF);

	headerBuffer.append(frameSize&0xFF).append((frameSize>>8)&0xFF);

	headerBuffer.append(bitsPerSample&0xFF).append((bitsPerSample>>8)&0xFF);

	headerBuffer.append("data", 4);

	headerBuffer.append(length&0xFF).append((length>>8)&0xFF);
	headerBuffer.append((length>>16)&0xFF).append((length>>24)&0xFF);

	QString audioFilename = QDir::homePath() + "/morse.wav";

	cout << audioFilename.toStdString() << endl;

	// Write wav to disk
	QFile file(audioFilename);
	bool removeOk = file.remove();

	bool openOk = file.open(QIODevice::WriteOnly);

	if(openOk)
	{
		file.write(headerBuffer);

		QByteArray bufferMixed;
		bufferMixed.fill(0);

		for(int i = 0; i < bufferI->size(); i+=2)
		{
			char c = bufferI->at(i);
			bufferMixed.append(bufferI->at(i));
			bufferMixed.append(bufferI->at(i+1));
			bufferMixed.append(bufferQ->at(i));
			bufferMixed.append(bufferQ->at(i+1));
		}

		file.write(bufferMixed);
	}
	else
	{
		cout << "Could not open file '" << audioFilename.toStdString() << "' for writing!" << endl;
	}

	file.close();
}