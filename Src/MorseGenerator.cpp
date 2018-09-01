/*
Copyright (c) 2018 Teijo Hakala

See the file LICENSE.md for copying permission.

Version 1.0.1
*/

#include "MorseGenerator.h"
#include <iostream>
#include <QDataStream>
#include <QtCore/qmath.h>

using namespace std;

const qint16  PCMS16MaxValue     =  32767;
const quint16 PCMS16MaxAmplitude =  32768; // because minimum is -32768

MorseGenerator::MorseGenerator(double freq, double sampleRate, double fadeInAndOutMS, bool swapEndian) :
	phase(0.0),
	freq(freq),
	sampleRate(sampleRate),
	swapEndian(swapEndian),
	fadeInAndOutMS(fadeInAndOutMS),
	keying(nullptr)
{
}

void MorseGenerator::Reset()
{
	phase = 0.0;
}

bool MorseGenerator::GenerateMorse(const QString &text, QList<qint16> &samplesI, QList<qint16> &samplesQ)
{
	if(!keying)
	{
		cerr << "No keying. Please call SetupKeying first." << endl;
		return false;
	}

	cout << text.toStdString() << endl;

	for(int i = 0; i < text.length(); i++)
	{
		QChar c = text.at(i).toLower();

		if(c == ' ')
		{
			// Space between words
			GenerateTone(keying->wordSpace * keying->unitLengthMS, 0, samplesI, samplesQ);

			cout << endl;
		}
		else
		{
			QString code = keying->codes[c.toLatin1()];

			cout << code.toStdString();

			for(int p = 0; p < code.length(); p++)
			{
				if(code.at(p) == '.')
				{
					GenerateTone(keying->dotLength * keying->unitLengthMS, 1, samplesI, samplesQ);
				}
				else if(code.at(p) == '-')
				{
					GenerateTone(keying->dashLength * keying->unitLengthMS, 1, samplesI, samplesQ);
				}

				// Space between part of the same letter
				if(p != code.length() - 1)
				{
					GenerateTone(keying->partSpace * keying->unitLengthMS, 0, samplesI, samplesQ);
				}

				// Space between letters
				else if(i != text.length() - 1)
				{
					GenerateTone(keying->letterSpace * keying->unitLengthMS, 0, samplesI, samplesQ);
					cout << " ";
				}
			}
		}
	}

	cout << endl;

	return true;
}

bool MorseGenerator::GenerateMorse(const QString &text, QByteArray &samplesI, QByteArray &samplesQ)
{
	QList<qint16> samplesIList;
	QList<qint16> samplesQList;
	bool ok = GenerateMorse(text, samplesIList, samplesQList);

	samplesI.clear();
	samplesQ.clear();

	QDataStream strI(&samplesI, QIODevice::WriteOnly);
	strI << samplesIList;

	QDataStream strQ(&samplesQ, QIODevice::WriteOnly);
	strQ << samplesQList;

	// QDataStream writes (at least) data length to the beginning of destination array, so remove that
	samplesI.remove(0, samplesI.size() - samplesIList.size() * 2);
	samplesQ.remove(0, samplesQ.size() - samplesQList.size() * 2);

	return ok;
}

void MorseGenerator::GenerateTone(double toneLengthMS, double amplitude, QList<qint16> &valuesI, QList<qint16> &valuesQ)
{
	double sampleLengthMS = (1.0 / sampleRate) * 1e3;
	double sampleCount = (toneLengthMS / sampleLengthMS);

	double d = 2.0 * M_PI / sampleRate;
	double phaseStep = d * freq;

	for(int s = 0; s < (int) sampleCount; s++)
	{
		double amplitudeToUse = amplitude;
		double posInToneMS = (double) s * sampleLengthMS;

		// Fade in and out
		if(fadeInAndOutMS != 0.0)
		{
			if(posInToneMS <= fadeInAndOutMS)
			{
				amplitudeToUse *= (posInToneMS / fadeInAndOutMS);
			}

			if(posInToneMS >= toneLengthMS - fadeInAndOutMS)
			{
				amplitudeToUse *= ((toneLengthMS - posInToneMS) / fadeInAndOutMS);
			}
		}

		qreal i = amplitudeToUse * qSin(phase);
		qreal q = amplitudeToUse * qSin(phase - qDegreesToRadians(90.0));
		qint16 pcmI = RealToPcm(i);
		qint16 pcmQ = RealToPcm(q);

		if(swapEndian)
		{
			qint16 swappedI = (pcmI >> 8) | (pcmI << 8);
			qint16 swappedQ = (pcmQ >> 8) | (pcmQ << 8);
			valuesI.push_back(swappedI);
			valuesQ.push_back(swappedQ);
		}
		else
		{
			valuesI.push_back(pcmI);
			valuesQ.push_back(pcmQ);
		}

		phase += phaseStep;
		phase = fmod(phase, 2 * M_PI);
	}
}

qint16 MorseGenerator::RealToPcm(qreal real)
{
    return real * PCMS16MaxValue;
}
