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


#ifndef ENUMTYPE_H
#define ENUMTYPE_H

#include "MdiWindowType.h"

class EnumType : public MdiWindowType {
    Q_OBJECT
    Q_INTERFACES(InterfaceWidgetPlugin)
public:
    explicit EnumType(QObject *parent = nullptr);
    ~EnumType() Q_DECL_OVERRIDE;

    virtual MdiWindowWidget* newElement(MdiSubWindow* par_SubWindow) Q_DECL_OVERRIDE;
    virtual MdiWindowWidget* openElement(MdiSubWindow* par_SubWindow, QString &arg_WindowName) Q_DECL_OVERRIDE;
    virtual QStringList openElementDialog() Q_DECL_OVERRIDE;

};


#endif // ENUMTYPE_H
