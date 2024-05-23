/*
 * Copyright 2023 ZF Friedrichshafen AG
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef A2LCALSINGLETYPE_H
#define A2LCALSINGLETYPE_H

#include <QObject>
#include <QDrag>
#include <QMimeData>
#include "MdiWindowType.h"

class A2LCalSingleType : public MdiWindowType{
    Q_OBJECT
    Q_INTERFACES(InterfaceWidgetPlugin)
public:
    explicit A2LCalSingleType(QObject *arg_parent = nullptr);
    ~A2LCalSingleType() Q_DECL_OVERRIDE;

    virtual MdiWindowWidget* newElement(MdiSubWindow* par_SubWindow) Q_DECL_OVERRIDE;
    virtual MdiWindowWidget* openElement(MdiSubWindow* par_SubWindow, QString &arg_WindowName) Q_DECL_OVERRIDE;
    virtual QStringList openElementDialog() Q_DECL_OVERRIDE;

private:
    struct settings {
        int m_width;
        int m_height;
        QString m_name;
        QString m_variableName;
        QColor m_color;
    };

    QMap<QString, settings> *m_texts;
};

#endif // A2LCALSINGLETYPE_H
