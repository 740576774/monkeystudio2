#include "pDockWidget.h"

#include <QStyle>

pDockWidget::pDockWidget( const QString& t, QWidget* w, Qt::WindowFlags f )
	: QDockWidget( t, w, f )
{}

pDockWidget::pDockWidget( QWidget* w, Qt::WindowFlags f )
	: QDockWidget( w, f )
{}

QSize pDockWidget::contentsSize() const

QSize pDockWidget::sizeHint() const
{ return mSize.isValid() && !isFloating() ? mSize : QDockWidget::sizeHint(); }

void pDockWidget::setVisible( bool b )
{
	if ( !b && !isFloating() )
}