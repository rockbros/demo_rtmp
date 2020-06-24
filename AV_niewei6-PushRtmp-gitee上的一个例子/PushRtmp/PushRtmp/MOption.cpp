/*
The MIT License (MIT)

Copyright (c) wenjie.zhao

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "MOption.h"

#include <QCoreApplication>
#include <QStringList>
#include <QColor>
#include <QDir>

/// define ini file name and path
#define SETTING_FILE "/setting.ini"
MOption* MOption::m_theOptions = 0;

MOption::MOption(QObject *parent) :
    QObject(parent)
{
    QString settingsPath = QCoreApplication::applicationDirPath() + SETTING_FILE;
    m_settings = new QSettings(settingsPath, QSettings::IniFormat, this);
}

MOption::~MOption()
{
    delete m_settings;

    destroy();
}

MOption* MOption::instance()
{
    if(!m_theOptions)
        m_theOptions = new MOption;

    return m_theOptions;
}

void MOption::destroy()
{
    if(m_theOptions)
    {
        delete m_theOptions;
        m_theOptions = 0;
    }
}

void MOption::setOption(const QVariant &value, const QString &key, const QString &group)
{
    m_settings->beginGroup(group);
    m_settings->setValue(key, value);
    m_settings->endGroup();
    m_settings->sync();
}

QVariant MOption::option(const QString &key, const QString &group)
{
    QVariant v;

    /// add
    m_mutex.lock();
    m_settings->beginGroup(group);
    v = m_settings->value(key, QVariant());
    m_settings->endGroup();
    m_mutex.unlock();

    return v;
}
