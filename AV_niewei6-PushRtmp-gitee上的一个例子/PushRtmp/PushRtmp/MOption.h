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

#ifndef MOption_H
#define MOption_H

#include <QObject>
#include <QVariant>
#include <QSettings>
#include <QMutex>
#include <QHash>

class MOption : public QObject
{
    Q_OBJECT
protected:
    explicit MOption(QObject *parent = 0);
    ~MOption();

public:
    QVariant option(const QString &key, const QString &group);
    void setOption(const QVariant& value, const QString &key, const QString &group);
    static MOption* instance();
    static void destroy();

private:
    static MOption *m_theOptions;

    QSettings *m_settings;
    QHash<QString, QVariant> m_defaultValue;

    QMutex m_mutex;

};

#endif // MOption_H
