/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * Flacon - audio File Encoder
 * https://github.com/flacon/flacon
 *
 * Copyright: 2022
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

#include "alacencoder.h"
#include <taglib/mp4file.h>
#include <taglib/tpropertymap.h>

QStringList AlacEncoder::programArgs() const
{
    QStringList args;
    args << programPath();

    args << "--quiet"; // Produce no output to stderr

    // Settings .................................................
    if (profile().value("Compression").toInt() == 0) {
        args << QString("--fast");
    }

    args << "-";
    args << outFile();
    return args;
}

static TagLib::MP4::CoverArt::Format coverFormatToTagLib(const CoverImage::Format fmt)
{

    // clang-format off
    switch (fmt) {
        case CoverImage::Format::JPG: return TagLib::MP4::CoverArt::Format::JPEG;
        case CoverImage::Format::PNG: return TagLib::MP4::CoverArt::Format::PNG;
        case CoverImage::Format::BMP: return TagLib::MP4::CoverArt::Format::BMP;
        case CoverImage::Format::GIF: return TagLib::MP4::CoverArt::Format::GIF;
        default:                      return TagLib::MP4::CoverArt::Format::Unknown;
    }
    // clang-format on
}

void AlacEncoder::writeMetadata(const QString &filePath) const
{
    TagLib::MP4::File file(filePath.toLocal8Bit(), false);

    if (!file.isValid()) {
        throw FlaconError("Can't open file");
    }

    TagLib::PropertyMap props = file.properties();

    auto writeStrTag = [&props](const QString &tagName, const QString &value) {
        if (!value.isEmpty()) {
            props.replace(TagLib::String(tagName.toStdString(), TagLib::String::UTF8), TagLib::String(value.toStdString(), TagLib::String::UTF8));
        }
    };

    writeStrTag("ARTIST", track().artist());
    writeStrTag("ALBUM", track().album());
    writeStrTag("GENRE", track().genre());
    writeStrTag("DATE", track().date());
    writeStrTag("TITLE", track().title());
    writeStrTag("ALBUMARTIST", track().albumArtist());
    writeStrTag("COMMENT", track().comment());
    writeStrTag("TRACKNUMBER", QString("%1/%2").arg(track().trackNum()).arg(track().trackCount()));
    writeStrTag("DISCNUMBER", QString("%1/%2").arg(track().discNum()).arg(track().discCount()));

    file.setProperties(props);

    if (!coverImage().isEmpty()) {
        const CoverImage  &img = coverImage();
        TagLib::ByteVector data(img.data().data(), img.data().size());

        TagLib::MP4::CoverArt cover(coverFormatToTagLib(img.format()), data);
        file.tag()->setItem("covr", TagLib::MP4::CoverArtList().append(cover));
    }

    if (!file.save()) {
        throw FlaconError("Can't save file");
    }
}
