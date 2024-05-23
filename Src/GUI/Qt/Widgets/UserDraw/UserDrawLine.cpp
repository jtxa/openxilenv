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


#include "StringHelpers.h"
#include "UserDrawLine.h"

extern "C" {
#include "ThrowError.h"
}

// Syntax: Group(active, scale, rot) {
//             LineColor(color_red, color_green, color_blue)
//             FillColor(color(red, green, blue))
//             Line(width=1, x0, y0, x1, y2, ...., xn, yn)
//         }

UserDrawLine::UserDrawLine(int par_Pos, QString &par_ParameterString, UserDrawElement *par_Parent) : UserDrawElement(par_Parent)
{
    InitParameter(par_Pos, par_ParameterString);
}

UserDrawLine::~UserDrawLine()
{
    foreach(BaseValue *Value, m_XY) {
        delete (Value);
    }
    m_XY.clear();
}

void UserDrawLine::ResetToDefault()
{
    UserDrawElement::ResetToDefault();
    m_Width.SetFixed(1.0);
    m_XY.clear();
    m_Properties.Add(QString("points"), QString(), UserDrawPropertiesListElem::UserDrawPropertyPointsSignalType);
}

void UserDrawLine::SetDefaultParameterString()
{
    m_ParameterLine = QString("(x=0,y=0,points=((0.1,0.1),(0.9,0.1),(0.9,0.9)))");
}

bool UserDrawLine::ParseOneParameter(QString &par_Value)
{
    QStringList List = par_Value.split('=');
    if (List.size() == 2) {
        QString Key = List.at(0).trimmed();
        QString ValueString = List.at(1).trimmed();
        if (!Key.compare("width", Qt::CaseInsensitive) ||
            !Key.compare("w", Qt::CaseInsensitive)) {
            m_Width.Parse(ValueString);
            m_Properties.Add("width", ValueString);
        } else if (!Key.compare("points", Qt::CaseInsensitive) ||
                   !Key.compare("p", Qt::CaseInsensitive)) {
            int Len = ValueString.length();
            if ((Len > 0) && (ValueString.at(0) != '(')) {
                ThrowError (1, "expecting a '(' in \"%s\"", QStringToConstChar(ValueString));
                return false;
            }
            int BraceCounter = 0;
            int StartParameter = 0;
            for (int x = 0; x < Len; x++) {
                QChar c = ValueString.at(x);
                if (c == '(') {
                    BraceCounter++;
                    if (BraceCounter == 1) {
                        StartParameter = x;
                    }
                } else if (c == ')') {
                    if ((BraceCounter == 0) && (x < (Len - 1))) {
                        ThrowError (1, "missing '(' in \"%s\"", QStringToConstChar(ValueString));
                        return false;
                    }
                    if (BraceCounter == 1) {
                        QString Parameter = ValueString.mid(StartParameter+1, x - StartParameter-1);
                        ParseOnePoint(Parameter);
                        //BaseValue *Value = new BaseValue(Parameter);
                        //m_XY.append(Value);
                        StartParameter = x;
                    }
                    BraceCounter--;
                } else if (c == ',') {
                    if (BraceCounter == 1) {
                        QString Parameter = ValueString.mid(StartParameter+1, x - StartParameter-1);
                        ParseOnePoint(Parameter);
                        //BaseValue *Value = new BaseValue(Parameter);
                        //m_XY.append(Value);
                        StartParameter = x;
                    }
                }
            }
            m_Properties.Add("points", ValueString, UserDrawPropertiesListElem::UserDrawPropertyPointsSignalType);
        } else {
            return ParseOneBaseParameter(Key, ValueString);
        }
        return true;
    }
    return false;
}

void UserDrawLine::Paint(QPainter *par_Painter, QList<UserDrawImageItem*> *par_Images, DrawParameter &par_Draw)
{
    Q_UNUSED(par_Images)
    if (m_Visible.Get() >= 0.5) {
        SetColor(par_Painter);
        int Count = m_XY.size() / 2;
        TranslateParameter P(m_X.Get(),
                             m_Y.Get(),
                             m_Scale.Get(),
                             m_Rot.Get(),
                             par_Draw);
        double xl, yl;
        for (int i = 0; i < Count; i++) {
            BaseValue *Value = m_XY.at(i<<1);
            double x = Value->Get();
            Value = m_XY.at((i<<1) + 1);
            double y = Value->Get();
            Translate(&x, &y, P);
            if (i) {
                QLineF Line(xl, yl, x, y);
                par_Painter->drawLine(Line);
            }
            xl = x;
            yl = y;
        }
    }
}

QString UserDrawLine::GetTypeString()
{
    return QString("Line");
}

int UserDrawLine::Is(QString &par_Line)
{
    if (par_Line.startsWith("Line", Qt::CaseInsensitive)) {
        return 4;
    } else {
        return 0;
    }
}

bool UserDrawLine::ParseOnePoint(QString &par_Point)
{
    int Len = par_Point.length();
    if ((Len > 0) && (par_Point.at(0) != '(')) {
        ThrowError (1, "expecting a '(' in \"%s\"", QStringToConstChar(par_Point));
        return false;
    }
    int BraceCounter = 0;
    int StartParameter = 0;
    for (int x = 0; x < Len; x++) {
        QChar c = par_Point.at(x);
        if (c == '(') {
            BraceCounter++;
            if (BraceCounter == 1) {
                StartParameter = x;
            }
        } else if (c == ')') {
            if ((BraceCounter == 0) && (x < (Len - 1))) {
                ThrowError (1, "missing '(' in \"%s\"", QStringToConstChar(par_Point));
                return false;
            }
            if (BraceCounter == 1) {
                QString Parameter = par_Point.mid(StartParameter+1, x - StartParameter-1);
                BaseValue *Value = new BaseValue(Parameter);
                m_XY.append(Value);
                StartParameter = x;
            }
            BraceCounter--;
        } else if (c == ',') {
            if (BraceCounter == 1) {
                QString Parameter = par_Point.mid(StartParameter+1, x - StartParameter-1);
                BaseValue *Value = new BaseValue(Parameter);
                m_XY.append(Value);
                StartParameter = x;
            }
        }
    }
    return true;
}

