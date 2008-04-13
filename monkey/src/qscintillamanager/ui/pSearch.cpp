/****************************************************************************
**
**         Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pSearch.cpp
** Date      : 2008-01-14T00:37:03
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
#include "pSearch.h"
#include "../../coremanager/MonkeyCore.h"
#include "../../workspace/pWorkspace.h"
#include "../../xupmanager/ui/UIXUPManager.h"
#include "../../workspace/pChild.h"
#include "../pEditor.h"
#include "../../maininterface/UIMain.h"
#include "../SearchThread.h"


#include <qscintilla.h>

#include <QKeyEvent>
#include <QDir>
#include <QStatusBar>
#include <QFileDialog>

#include <QGridLayout>
#include <QToolButton>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QMessageBox>

#include <QDebug>

pSearch::pSearch( QWidget* parent )
	: QWidget( parent )
{
	layout = new QGridLayout (this);
	layout->setContentsMargins (0, 0, 0, 0);
	layout->setSpacing (2);
	//search
	lSearchText = new QLabel (tr("Searc&h:"));
	lSearchText->setSizePolicy (QSizePolicy::Maximum, QSizePolicy::Fixed);
	lSearchText->setAlignment (Qt::AlignVCenter | Qt::AlignRight);
	
	cobSearch = new QComboBox ();
	cobSearch->setEditable (true);
	cobSearch->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
	lSearchText->setBuddy (cobSearch);

	tbPrevious = new QPushButton ();
	tbPrevious->setText (tr("&Previous"));
	tbPrevious->setIcon (QIcon (":/edit/icons/edit/previous.png"));
	tbPrevious->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
	tbPrevious->setFlat (true);
	
	tbNext = new QPushButton ();
	tbNext->setText (tr("&Next"));
    tbNext->setIcon (QIcon(":/edit/icons/edit/next.png"));
	tbNext->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
	tbNext->setFlat (true);
	
	cbCaseSensitive = new QCheckBox ();
	cbCaseSensitive->setText (tr("&Case"));
	cbCaseSensitive->setSizePolicy (QSizePolicy::Maximum, QSizePolicy::Fixed);
	
	cbRegExp = new QCheckBox ();
	cbRegExp->setText (tr("Re&gExp"));
	cbRegExp->setSizePolicy (QSizePolicy::Maximum, QSizePolicy::Fixed);
	
	//replace
	lReplaceText = new QLabel (tr("R&eplace:"));
	lReplaceText->setAlignment (Qt::AlignVCenter | Qt::AlignRight);
	
	cobReplace = new QComboBox ();
	cobReplace->setEditable (true);
	cobReplace->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
	lReplaceText->setBuddy (cobReplace);
	
	tbReplace = new QPushButton ();
	tbReplace->setText (tr("&Replace"));
	tbReplace->setIcon (QIcon (":/edit/icons/edit/replace.png"));
	tbReplace->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
	tbReplace->setFlat (true);
	
	tbReplaceAll = new QPushButton ();
	tbReplaceAll->setText (tr("Replace &all"));
	tbReplaceAll->setIcon (QIcon (":/edit/icons/edit/replace.png"));
	tbReplaceAll->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
	tbReplaceAll->setFlat (true);

	//folder
	lPath = new QLabel  (tr("&Path:"));
	lPath->setAlignment (Qt::AlignVCenter | Qt::AlignRight);
	
	cobPath = new QComboBox();
	cobPath->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
	cobPath->setEditable (true);
	lPath->setBuddy (cobPath);
	
	tbPath = new QToolButton ();
	tbPath->setText ("...");
	lMask = new QLabel  (tr("&Mask:"));
	lMask->setAlignment (Qt::AlignVCenter | Qt::AlignRight);
	
	cobMask = new QComboBox ();
	cobMask->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
	cobMask->setEditable (true);
	cobMask->setToolTip (tr("Spase separated list of wildcards. Example:<br> <i>*.h *.cpp file???.txt</i>"));
	lMask->setBuddy (cobMask);
	
	connect (tbNext, SIGNAL (clicked()), this, SLOT (on_tbNext_clicked ()));
	connect (tbPrevious, SIGNAL (clicked()), this, SLOT (on_tbPrevious_clicked ()));
	connect (tbReplace, SIGNAL (clicked()), this, SLOT (on_tbReplace_clicked ()));
	connect (tbReplaceAll, SIGNAL (clicked()), this, SLOT (on_tbReplaceAll_clicked ()));
	
    qRegisterMetaType<pConsoleManager::Step>("pConsoleManager::Step");
    connect( MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aSearchFile" ), SIGNAL( triggered() ), SLOT( showSearchFile() ) );
    connect( MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aReplaceFile" ), SIGNAL( triggered() ), SLOT( showReplaceFile() ) );
    //connect( MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aSearchProject" ), SIGNAL( triggered() ), SLOT( showSearchProject() ) );
    //connect( MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aReplaceProject" ), SIGNAL( triggered() ), SLOT( showReplaceProject() ) );
    connect( MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aSearchFolder" ), SIGNAL( triggered() ), SLOT( showSearchFolder() ) );
    //connect( MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aReplaceFolder" ), SIGNAL( triggered() ), SLOT( showReplaceFolder() ) );
    connect( MonkeyCore::menuBar()->action( "mEdit/mSearchReplace/aSearchNext" ), SIGNAL( triggered() ), SLOT( on_tbNext_clicked() ) );
    
	connect( tbPath, SIGNAL( clicked() ), this, SLOT( on_tbPath_clicked() ) );
    
	mSearchThread = NULL;
	
	setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Maximum);
    
	addSearchToLayout (0);
	mOperType = SEARCH;
	mWhereType = FILE;
}

void pSearch::addSearchToLayout (int row)
{
	layout->addWidget (lSearchText, row, 0, 1, 1);
	layout->addWidget (cobSearch, row, 1, 1, 2);
	layout->addWidget (tbPrevious, row, 3, 1, 1);
	layout->addWidget (tbNext, row, 4, 1, 1);
	layout->addWidget (cbCaseSensitive, row, 5, 1, 1);
	layout->addWidget (cbRegExp, row, 6, 1, 1);
}

void pSearch::addReplaceToLayout (int row)
{
	layout->addWidget (lReplaceText, row, 0, 1, 1);
	layout->addWidget (cobReplace, row, 1, 1, 2);
	layout->addWidget (tbReplace, row, 3, 1, 1);
	layout->addWidget (tbReplaceAll, row, 4, 1, 1);
	
	lReplaceText->show();
	cobReplace->show();
	tbReplace->show();
	tbReplaceAll->show();
}

void pSearch::addFolderToLayout (int row)
{
	layout->addWidget (lPath, row, 0, 1, 1);
	layout->addWidget (cobPath, row, 1, 1, 1);
	layout->addWidget (tbPath, row, 2, 1, 1);
	layout->addWidget (lMask, row, 3, 1, 1);
	layout->addWidget (cobMask, row, 4, 1, 3);
	
	lPath->show ();
	cobPath->show ();
	tbPath->show ();
	lMask->show ();
	cobMask->show ();
}

void pSearch::removeSearchFromLayout ()
{
	layout->removeWidget (lSearchText);
	layout->removeWidget(cobSearch);
	layout->removeWidget (tbPrevious);
	layout->removeWidget (tbNext);
	layout->removeWidget (cbCaseSensitive);
	layout->removeWidget (cbRegExp);
}

void pSearch::removeReplaceFromLayout ()
{
	layout->removeWidget (lReplaceText);
	layout->removeWidget (cobReplace);
	layout->removeWidget (tbReplace);
	layout->removeWidget (tbReplaceAll);
	
	lReplaceText->hide();
	cobReplace->hide();
	tbReplace->hide();
	tbReplaceAll->hide();
}

void pSearch::removeFolderFromLayout ()
{
	layout->removeWidget (lPath);
	layout->removeWidget (cobPath);
	layout->removeWidget (tbPath);
	layout->removeWidget (lMask);
	layout->removeWidget (cobMask);
	
	lPath->hide ();
	cobPath->hide ();
	tbPath->hide ();
	lMask->hide ();
	cobMask->hide ();
}

bool pSearch::isProjectAvailible ()
{
    if (MonkeyCore::projectsManager()->currentProject())
        return true;
    else
        return false;
}

void pSearch::keyPressEvent( QKeyEvent* e )
{
    switch (e->key())
    {
        case Qt::Key_Escape:
            hide();
			MonkeyCore::workspace()->focusToEditor_triggered ();
        break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            if (mOperType == SEARCH)
                on_tbNext_clicked();
            else /* replace */
                on_tbReplace_clicked ();
    }
    QWidget::keyPressEvent( e );
}


void pSearch::showSearchFile () 
{
	if (mOperType == REPLACE)
		removeReplaceFromLayout ();
	mOperType = SEARCH;
	
	if (mWhereType == FOLDER)
		removeFolderFromLayout ();
	mWhereType = FILE;
	
    show ();
};

void pSearch::showReplaceFile () 
{
	if (mWhereType == FOLDER)
		removeFolderFromLayout ();
	mWhereType = FILE;
	
	if (mOperType == SEARCH)
		addReplaceToLayout(1);
	mOperType = REPLACE;
	
    show ();
};

/*
void pSearch::showSearchProject () 
{
    mOperType = SEARCH;
    mWhereType = PROJECT;
    show ();
};

void pSearch::showReplaceProject () 
{
    mOperType = REPLACE;
    mWhereType = PROJECT;
    show ();
};
*/

void pSearch::showSearchFolder () 
{
	if (mOperType == REPLACE)
		removeReplaceFromLayout ();
	mOperType = SEARCH;
	
	if (mWhereType == FILE)
		addFolderToLayout(1);
	mWhereType = FOLDER;
	cobPath->lineEdit()->setText (QDir::current().absolutePath ());
	
    show ();
};

/*void pSearch::showReplaceFolder () 
{
	if (mOperType == SEARCH)
		addReplaceToLayout (1);
	mOperType = REPLACE;
	
	if (mWhereType == FILE)
		addFolderToLayout (2);
	mWhereType = FOLDER;
	
    show ();
};
*/
void pSearch::show ()
{
    pChild* child = dynamic_cast<pChild*> (MonkeyCore::workspace()->currentChild());
    if (child && child->editor())
    {
		pEditor* editor = child->editor ();
        QString text = editor->selectedText ();
		if (!text.isNull() && text.indexOf ('\n') == -1)
			cobSearch->lineEdit()->setText (text);
    }
	
	cobSearch->setFocus();
	cobSearch->lineEdit()->selectAll ();    
	
	if (mWhereType == FILE)
	{
		tbNext->setText (tr("&Next"));
		tbNext->setIcon (QIcon(":/edit/icons/edit/next.png"));
		tbPrevious->show();
	}
	else
	{
		tbNext->setText (tr("&Search"));
		tbNext->setIcon (QIcon(":/edit/icons/edit/search.png"));
		tbPrevious->hide();
	}
	
    QWidget::show ();
}

bool pSearch::searchFile (bool next, bool wrap)
{
	QString text = cobSearch->currentText ();
	
	searchAddToRecents(text);
	
    pChild* child = dynamic_cast<pChild*> (MonkeyCore::workspace()->currentChild());
    if (!child || !child->editor())
    {
        showMessage(tr( "No active editor" ) );
        return false;
    }
    pEditor* editor = child->editor ();

    // get cursor position
    int x, y;
    editor->getCursorPosition( &y, &x );

    if (!next)
    {
        int temp;
        editor->getSelection(&y, &x, &temp, &temp);
    }

    // search
    bool b = editor->findFirst( text, cbRegExp->isChecked(), cbCaseSensitive->isChecked(), false, wrap, next, y, x);

    // change background acording to found or not
    QPalette p = cobSearch->palette();
    p.setColor( cobSearch->backgroundRole(), b ? Qt::white : Qt::red );
    cobSearch->setPalette( p );
    
    // show message if needed
    showMessage( b ? QString::null : tr( "Not Found" ) );

    // return found state
    return b;
}

bool pSearch::on_tbPrevious_clicked()
{
    return searchFile (false);
}

bool pSearch::on_tbNext_clicked()
{
	
    switch (mWhereType)
    {
        case FILE:
			if (!isPathValid ())
				return false;
            return searchFile (true);
        case FOLDER:
			if (!isSearchTextValid () || !isPathValid ())
				return false;
            if (mSearchThread && mSearchThread->isRunning ())
            { // need to stop searching
                mSearchThread->setTermEnabled (true);
            }
            else
            { // need to start searching
				if (!QDir(cobPath->currentText()).exists())
				{
					showMessage (tr("Invalid path"));
					return false;
				}
                emit clearSearchResults ();
				pathAddToRecents (cobPath->currentText());
				maskAddToRecents (cobMask->currentText());
                mOccurencesFinded = 0;
                mFilesProcessed = 0;
                fileProcessed (0);
                QString path = cobPath->currentText();
                QString mask = cobMask->currentText();
                QString text = cobSearch->currentText();
                //bool whole = cbWholeWords->isChecked ();
                bool match = cbCaseSensitive->isChecked();
                bool regexp = cbRegExp->isChecked ();
                mSearchThread = new SearchThread(path, mask, text, true, match, regexp, this);
                tbNext->setText (tr("&Stop"));
                tbNext->setIcon (QIcon(":/console/icons/console/stop.png"));
                
                connect (mSearchThread, SIGNAL (appendSearchResult( const pConsoleManager::Step& )), MonkeyCore::workspace(), SIGNAL (appendSearchResult( const pConsoleManager::Step& )));
                connect (mSearchThread, SIGNAL (finished ()), this, SLOT (threadFinished()));
                connect (mSearchThread, SIGNAL (appendSearchResult( const pConsoleManager::Step& )), this, SLOT (occurenceFinded ()));
                connect (mSearchThread, SIGNAL (changeProgress(int)), this, SLOT (fileProcessed (int)));
                mSearchThread->start();    
            }
        break;
    }
    return true;
}

//
int pSearch::replace(bool all)
{
	QString rtext = cobReplace->currentText();
	
	searchAddToRecents(cobSearch->currentText());
	replaceAddToRecents(rtext);
	
    pChild* child = dynamic_cast<pChild*> (MonkeyCore::workspace()->currentChild());
    if (!child && !child->editor())
    {
        showMessage(tr( "No active editor" ) );
        return 0;
    }
    pEditor* editor = child->editor ();


    int count = 0;
	if (all)
	{
	    int x, y;
		editor->getCursorPosition(&y, &x);

		editor->setCursorPosition(0, 0);
        while (searchFile(true, false)) //search next, wrap switched off
        {
            editor->replace(rtext);
            count++;
        };
		editor->setCursorPosition(y, x); //restore cursor position
	}
	else
	{ 		//do replace one time
		 int x, y, temp;
		editor->getSelection(&y, &x, &temp, &temp);
		editor->setCursorPosition(y, x);

		if (on_tbNext_clicked())
			{
				editor->replace (rtext);
				count = 1;
				editor->findNext(); //move selection to next item
			}
	}
    
    return count;
}

void pSearch::on_tbReplace_clicked()
{
	/* Check replace text */
	if (!isReplaceTextValid ())
		return;

    replace (false);
}
//
void pSearch::on_tbReplaceAll_clicked()
{
	/* Check replace text */
	if (!isReplaceTextValid ())
		return;

    pChild* child = dynamic_cast<pChild*> (MonkeyCore::workspace()->currentChild());
    if (!child && !child->editor())
        return;
    pEditor* editor = child->editor ();
    
    // begin undo global action
    editor->beginUndoAction();

    int count = replace (true);
    // end undo global action
    editor->endUndoAction();

    // show occurence number replaced
    showMessage( count ? tr( "%1 occurences replaced" ).arg( count ) : tr( "Nothing To Repalce" ) );
}

void pSearch::on_tbPath_clicked ()
{
	QString text = QFileDialog::getExistingDirectory ( this, tr("Search path"), cobPath->currentText(), 0);
	if (!text.isNull())
		cobPath->lineEdit ()->setText (text);
}

void pSearch::threadFinished ()
{
    tbNext->setText (tr("Search"));
    tbNext->setIcon (QIcon(":/edit/icons/edit/search.png"));
    delete mSearchThread;
    mSearchThread = NULL;
	showMessage ("Searching finished");
}

void pSearch::occurenceFinded ()
{
    mOccurencesFinded ++;
    showMessage(tr ("%1 files %2 occcurences").arg(mFilesProcessed).arg(mOccurencesFinded));
}

void pSearch::fileProcessed (int count)
{
    mFilesProcessed = count;
    showMessage (tr ("%1 files %2 occcurences").arg(mFilesProcessed).arg(mOccurencesFinded));
}

void pSearch::showMessage (QString status)
{
	if (!status.isNull())
		MonkeyCore::mainWindow()->statusBar()->showMessage (tr ("Search: %1").arg (status), 30000);
	else
		MonkeyCore::mainWindow()->statusBar()->showMessage ("");
}

void pSearch::searchAddToRecents (QString text)
{
	if (searchRecents.isEmpty() || searchRecents[0] != text)
	{
		int i;
		while ((i = searchRecents.indexOf(text)) != -1)
			searchRecents.removeAt (i);
		searchRecents.prepend (text);
		if (searchRecents.size() > 10)
		{
			searchRecents.removeLast ();
		}
		cobSearch->clear();
		cobSearch->addItems (searchRecents);
	}
}

void pSearch::replaceAddToRecents (QString text)
{
	if (replaceRecents.isEmpty() || replaceRecents[0] != text)
	{
		int i;
		while ((i = replaceRecents.indexOf(text)) != -1)
			replaceRecents.removeAt (i);
		replaceRecents.prepend (text);
		if (replaceRecents.size() > 10)
		{
			replaceRecents.removeLast ();
		}
		cobReplace->clear();
		cobReplace->addItems (replaceRecents);
	}
}

void pSearch::pathAddToRecents (QString text)
{
	if (pathRecents.isEmpty() || pathRecents[0] != text)
	{
		int i;
		while ((i = pathRecents.indexOf(text)) != -1)
			pathRecents.removeAt (i);
		pathRecents.prepend (text);
		if (pathRecents.size() > 10)
		{
			pathRecents.removeLast ();
		}
		cobPath->clear();
		cobPath->addItems (pathRecents);
	}
}

void pSearch::maskAddToRecents (QString text)
{
	if (maskRecents.isEmpty() || maskRecents[0] != text)
	{
		int i;
		while ((i = maskRecents.indexOf(text)) != -1)
			maskRecents.removeAt (i);
		maskRecents.prepend (text);
		if (maskRecents.size() > 10)
		{
			maskRecents.removeLast ();
		}
		cobMask->clear();
		cobMask->addItems (maskRecents);
	}
}

bool pSearch::isSearchTextValid ()
{
	if (cbRegExp->isChecked())
		if (!QRegExp (cobSearch->currentText()).isValid())
		{
			QMessageBox::critical (MonkeyCore::workspace(), tr("Invalid regular expression"), tr ("Regular expression is invalid"));
			return false;
		}
	
	return true;
}

bool pSearch::isReplaceTextValid ()
{
	return true;
}

bool pSearch::isPathValid ()
{
	if (!QDir (cobPath->currentText()).exists())
	{
		QMessageBox::critical (MonkeyCore::workspace(), tr("Invalid path"), tr ("Search path not exist"));
		return false;
	}
	return true;
}

bool pSearch::isMaskValid ()
{
	return true;
}
