#include "StatusBar.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QIcon>

StatusBar::StatusBar( QWidget* parent )
	: QStatusBar( parent )
{
	// create labels
	QLabel* l;
	l = ( mLabels[ltCursorPosition] = new QLabel( this ) );
	l->setMargin( 2 );
	l->setToolTip( tr( "Cursor position" ) );
	l = ( mLabels[ltSaveState] = new QLabel( this ) );
	l->setMargin( 2 );
	l->setToolTip( tr( "Modification state of file" ) );
	l = ( mLabels[ltEOLMode] = new QLabel( this ) );
	l->setMargin( 2 );
	l->setToolTip( tr( "EOL mode" ) );
	l = ( mLabels[ltIndentMode] = new QLabel( this ) );
	l->setMargin( 2 );
	l->setToolTip( tr( "Indentation mode" ) );
	// add labels
	addPermanentWidget( mLabels[ltCursorPosition] );
	addPermanentWidget( mLabels[ltSaveState] );
	addPermanentWidget( mLabels[ltEOLMode] );
	addPermanentWidget( mLabels[ltIndentMode] );
	// connections
	connect( this, SIGNAL( messageChanged( const QString& ) ), this, SLOT( setMessage( const QString& ) ) );
}

QLabel* StatusBar::label( StatusBar::LabelType type )
{
	return mLabels[type];
}

void StatusBar::setMessage( const QString& message )
{
	showMessage( message );
	setToolTip( message );
}

void StatusBar::setModified( bool modified )
{
	label( ltSaveState )->setPixmap( QIcon( QPixmap( ":/file/icons/file/save.png" ) ).pixmap( QSize( 16, 16 ), modified ? QIcon::Normal : QIcon::Disabled ) );
}

void StatusBar::setEOLMode( QsciScintilla::EolMode mode )
{
	switch ( mode )
	{
		case QsciScintilla::EolWindows:
			label( ltEOLMode )->setText("Windows");
			break;
		case QsciScintilla::EolUnix:
			label( ltEOLMode )->setText("Unix");
			break;
		case QsciScintilla::EolMac:
			label( ltEOLMode )->setText("Mac");
			break;
		default:
			label( ltEOLMode )->setText("-");
			break;
	}
}

void StatusBar::setIndentMode( int mode )
{
	switch ( mode )
	{
		case 0:
			label( ltIndentMode )->setText("Spaces" );
			break;
		case 1:
			label( ltIndentMode )->setText("Tabs" );
			break;
		default:
			label( ltIndentMode )->setText("-");
			break;
	}
}

void StatusBar::setCursorPosition( const QPoint& pos )
{
	QString s = tr( "Line: %1 Column: %2" );
	label( ltCursorPosition )->setText( pos == QPoint( -1, -1 ) ? s.arg( "-" ).arg( "-" ) : s.arg( pos.y() ).arg( pos.x() ) );
}
