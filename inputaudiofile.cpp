/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * Flacon - audio File Encoder
 * https://github.com/flacon/flacon
 *
 * Copyright: 2012-2013
 *   Alexander Sokoloff <sokoloff.a@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include "inputaudiofile.h"
#include "decoder.h"
#include "informat.h"
#include <settings.h>
#include <QProcess>
#include <QStringList>
#include <QByteArray>
#include <QTextStream>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

/************************************************

 ************************************************/
InputAudioFile_OLD::InputAudioFile_OLD(const QString &fileName) :
    mFileName(fileName),
    mFormat(nullptr),
    mSampleRate(0),
    mBitsPerSample(0),
    mDuration(0),
    mValid(false),
    mCdQuality(false)
{
    mValid = load();
}

/************************************************

 ************************************************/
InputAudioFile_OLD::InputAudioFile_OLD(const InputAudioFile_OLD &other)
{
    operator=(other);
}

/************************************************
 *
 ************************************************/
InputAudioFile_OLD &InputAudioFile_OLD::operator=(const InputAudioFile_OLD &other)
{
    mFileName      = other.mFileName;
    mValid         = other.mValid;
    mErrorString   = other.mErrorString;
    mSampleRate    = other.mSampleRate;
    mBitsPerSample = other.mBitsPerSample;
    mCdQuality     = other.mCdQuality;
    mDuration      = other.mDuration;
    mFormat        = other.mFormat;
    return *this;
}

/************************************************

 ************************************************/
bool InputAudioFile_OLD::load()
{
    if (mFileName == "") {
        qWarning() << "The audio file name is not set";
        mErrorString = QObject::tr("The audio file name is not set");
        return false;
    }

    if (!QFileInfo(mFileName).exists()) {
        qWarning() << QString("The audio file <b>\"%1\"</b> does not exist").arg(mFileName);
        mErrorString = QObject::tr("The audio file <b>\"%1\"</b> does not exist").arg(mFileName);
        return false;
    }

    try {
        Decoder dec;
        dec.open(mFileName);
        mFormat = dec.audioFormat();

        mSampleRate    = dec.wavHeader().sampleRate();
        mBitsPerSample = dec.wavHeader().bitsPerSample();
        mCdQuality     = dec.wavHeader().isCdQuality();
        mDuration      = dec.duration();
        return true;
    }
    catch (FlaconError &err) {
        mErrorString = QObject::tr("File <b>%1</b> is not a supported audio file. <br>"
                                   "<br>Verify that all required programs are installed and in your preferences.")
                               .arg(mFileName);
        mErrorString += ": ";
        mErrorString += err.what();
        return false;
    }
}

InputAudioFile::Data::Data()
{
}

InputAudioFile::Data::Data(const InputAudioFile::Data &other) :
    QSharedData(other),
    mFilePath(other.mFilePath),
    mFileName(other.mFileName),
    mErrorString(other.mErrorString),
    mFormat(other.mFormat),
    mSampleRate(other.mSampleRate),
    mBitsPerSample(other.mBitsPerSample),
    mDuration(other.mDuration),
    mValid(other.mValid),
    mCdQuality(other.mCdQuality)
{
}

void InputAudioFile::Data::load(const QString &filePath)
{
    mFilePath = filePath;

    if (mFilePath.isEmpty()) {
        qWarning() << "The audio file name is not set";
        mErrorString = QObject::tr("The audio file name is not set");
        mValid       = false;
        mFileName.clear();
        return;
    }

    QFileInfo fi(filePath);
    mFileName = fi.fileName();

    if (!fi.exists()) {
        qWarning() << QString("The audio file <b>\"%1\"</b> does not exist").arg(mFilePath);
        mErrorString = QObject::tr("The audio file <b>\"%1\"</b> does not exist").arg(mFilePath);
        mValid       = false;
        return;
    }

    try {
        Decoder dec;
        dec.open(mFilePath);
        mFormat = dec.audioFormat();

        mSampleRate    = dec.wavHeader().sampleRate();
        mBitsPerSample = dec.wavHeader().bitsPerSample();
        mCdQuality     = dec.wavHeader().isCdQuality();
        mDuration      = dec.duration();
        mValid         = true;
    }
    catch (FlaconError &err) {
        mErrorString = QObject::tr("File <b>%1</b> is not a supported audio file. <br>"
                                   "<br>Verify that all required programs are installed and in your preferences.")
                               .arg(mFilePath);
        mErrorString += ": ";
        mErrorString += err.what();
        mValid = false;
    }
}

InputAudioFile::InputAudioFile() :
    mData(new Data())
{
}

InputAudioFile::InputAudioFile(const QString &fileName) :
    mData(new Data())
{
    mData->load(fileName);
}

InputAudioFile::InputAudioFile(const InputAudioFile &other) :
    mData(other.mData)
{
}

InputAudioFile &InputAudioFile::operator=(const InputAudioFile &other)
{
    mData = other.mData;
    return *this;
}
