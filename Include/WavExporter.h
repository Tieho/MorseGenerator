/*
Copyright (c) 2018 Teijo Hakala

See the file LICENSE.md for copying permission.
*/

#ifndef __WavExporter_h__
#define __WavExporter_h__

#include <QByteArray>

class WavExporter
{
public:
	static void Export(QByteArray *bufferI, QByteArray *bufferQ, unsigned int sampleRate, unsigned int bitsPerSample);
};

#endif