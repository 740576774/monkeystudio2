/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Base Plugins
** FileName  : Entity.h
** Date      : 2008-01-14T00:40:06
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/

#ifndef ENTITY_H
#define ENTITY_H
#include <QString>
#include <QList>
#include <QTreeWidget>
#include <QObject>
#include <QDateTime>
#include <QIcon>

#include "Ctags.h"

enum EntityType { 
    NAMESPACE = 0x0001,
    CLASS = 0x0002,
    CONSTRUCTOR = 0x0004,
    DESTRUCTOR = 0x0008,
    ENUM = 0x0010,
    ENUMERATOR = 0x0020,
    EXTERN = 0x0040,
    FUNCTION = 0x0080,
    MACRO = 0x0100,
    MEMBER = 0x0200,
    PROTOTYPE = 0x0400,
    STRUCT = 0x0800,
    TYPEDEF = 0x1000,
    UNION = 0x2000,
    VARIABLE = 0x4000,

    ROOT_ENTITY,  //15  //top level entiry, possible file, or project

    UNKNOWN = 0x8000 //16
};

class Entity : public QTreeWidgetItem
{
private:
    static QIcon iNAMESPACE;
    static QIcon iCLASS;
    static QIcon iENUM;
    static QIcon iFUNCTION;
    static QIcon iMEMBER;
    static QIcon iSTRUCT;
    static QIcon iTYPEDEF;
    
public:
    Entity ( tagEntryInfo* entry, QString fileName, QDateTime time);
    Entity (EntityType ttype, QString tname);
    QDateTime updateTime ;
    QString name;
    EntityType type;
    QString file;
    int line;
    QString signature;

    Entity* child (int i);
    
    //function will create or update info about file in the entity

    //convert string kindName to the EntityType
    static EntityType getScopeType ( QString kindName );
    //convert TagEntry.kind  to the EntiryType
    static EntityType getEntityType ( char kind );
    static void initIcons ();
    void deleteFileInfo ( QString file, QDateTime olderThan );
    void updateSelf (tagEntryInfo* entry, QString fileName, QDateTime time);
    void updateGUI ();
};

#endif //ENTITY_H
