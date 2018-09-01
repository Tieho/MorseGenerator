/*
Copyright (c) 2018 Teijo Hakala

See the file LICENSE.md for copying permission.
*/

#include "MorseGenerator.h"
#include "WavExporter.h"
#include <QCoreApplication>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QBuffer>
#include <QDebug>

void GenerateMorse()
{
	double sampleRate = 8000;

	// Generate morse signal's I and Q to QByteArray. Param 'true' swaps endianess for audio output.
	MorseGenerator gen(800, sampleRate, 0.0, true);
	gen.SetupKeying<InternationalKeying>(100);

	QByteArray *arrI = new QByteArray();
	QByteArray *arrQ = new QByteArray();
	gen.GenerateMorse("This is a test", *arrI, *arrQ);

	WavExporter::Export(arrI, arrQ, sampleRate, 16);

	// Setup buffer for audio output (I)
	QBuffer *buffer = new QBuffer();
	buffer->setBuffer(arrI);
	buffer->open(QIODevice::ReadOnly);

	// Play audio
	QAudioFormat format;
	format.setSampleRate(sampleRate);
	format.setChannelCount(1);
	format.setSampleSize(16);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::SignedInt);

	QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());

	if(!info.isFormatSupported(format))
	{
		qWarning() << "Raw audio format not supported by backend, cannot play audio.";
		return;
	}

	QAudioOutput *audio = new QAudioOutput(format, 0);
	audio->start(buffer);
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	GenerateMorse();

	int ret = app.exec();
	return ret;
}
