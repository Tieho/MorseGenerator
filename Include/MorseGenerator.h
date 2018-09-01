/*
Copyright (c) 2018 Teijo Hakala

See the file LICENSE.md for copying permission.
*/

#ifndef __MorseGenerator_h_
#define __MorseGenerator_h_

#include <QList>
#include <QString>
#include <map>

class KeyingBase
{
public:
	KeyingBase() :
		dotLength(1),
		dashLength(3),
		partSpace(1),
		letterSpace(3),
		wordSpace(7),
		unitLengthMS(100)
	{
	}

	int dotLength;
	int dashLength;
	int partSpace;
	int letterSpace;
	int wordSpace;
	int unitLengthMS;

	std::map<char, QString> codes;
};

class InternationalKeying : public KeyingBase
{
public:
	InternationalKeying()
	{
		dotLength = 1;
		dashLength = 3;
		partSpace = 1;
		letterSpace = 3;
		wordSpace = 7;

		std::map<char, QString> morseCodes =
		{
			{ 'a', ".-" },
			{ 'b', "-..." },
			{ 'c', "-.-." },
			{ 'd', "-.." },
			{ 'e', "." },
			{ 'f', "..-." },
			{ 'g', "--." },
			{ 'h', "...." },
			{ 'i', ".." },
			{ 'j', ".---" },
			{ 'k', "-.-" },
			{ 'l', ".-.." },
			{ 'm', "--" },
			{ 'n', "-." },
			{ 'o', "---" },
			{ 'p', ".--." },
			{ 'q', "--.-" },
			{ 'r', ".-." },
			{ 's', "..." },
			{ 't', "-" },
			{ 'u', "..-" },
			{ 'v', "...-" },
			{ 'w', ".--" },
			{ 'x', "-..-" },
			{ 'y', "-.--" },
			{ 'z', "--.." },
			{ '1', ".----" },
			{ '2', "..---" },
			{ '3', "...--" },
			{ '4', "....-" },
			{ '5', "....." },
			{ '6', "-...." },
			{ '7', "--..." },
			{ '8', "---.." },
			{ '9', "----." },
			{ '0', "-----" },
		};

		codes = morseCodes;
	}
};

class MorseGenerator
{
public:
	MorseGenerator(double freq = 800.0, double sampleRate = 44100.0, double fadeInAndOutMS = 0.0, bool swapEndian = false);

	void Reset();
	bool GenerateMorse(const QString &text, QList<qint16> &samplesI, QList<qint16> &samplesQ = QList<qint16>());
	bool GenerateMorse(const QString &text, QByteArray &samplesI, QByteArray &samplesQ = QByteArray());

	template<class T> void SetupKeying(double unitLengthMS = 100.0)
	{
		keying = new T;
		keying->unitLengthMS = unitLengthMS;
	}

protected:
	void GenerateTone(double lengthMS, double amplitude, QList<qint16> &valuesI, QList<qint16> &valuesQ = QList<qint16>());
	qint16 RealToPcm(qreal real);

private:
	double phase;
	double freq;
	double sampleRate;
	bool swapEndian;
	double fadeInAndOutMS;
	KeyingBase *keying;
};

#endif
