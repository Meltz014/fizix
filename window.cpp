#include <QDebug>
#include "renderarea.h"
#include "window.h"
#include <QtWidgets>

Window::Window( )
{
   qDebug( ) << "Hello";
   renderArea = new RenderArea( this );

   newShapeSpinBox = new QSpinBox;
   newShapeSpinBox->setRange(0, NUM_SHAPES-1);

   newShapeLabel = new QLabel(tr("Shape:"));
   newShapeLabel->setBuddy(newShapeSpinBox);

   fpsCounterLabel = new QLabel( tr( "000.00 FPS" ) );

   // connect the param controls with associated slots
   // signal on valueChanged() of spin box, call penChanged() of this window
   connect( newShapeSpinBox, SIGNAL( valueChanged( int ) ), this, SLOT( penChanged(  ) ) );

   // create layout
   QGridLayout *mainLayout = new QGridLayout;
   mainLayout->setColumnStretch( 0, 1 );
   mainLayout->addWidget( renderArea, 0, 0, 1, 4 );
   mainLayout->addWidget( fpsCounterLabel, 1, 0, Qt::AlignLeft );
   mainLayout->addWidget( newShapeLabel, 1, 1, Qt::AlignRight );
   mainLayout->addWidget( newShapeSpinBox, 1, 2 );
   setLayout( mainLayout );

   //penChanged(  );
   setWindowTitle( tr( "Basic Drawing" ) );
}

void Window::penChanged(  )
{
   renderArea->setNewShape( (ShapeType) newShapeSpinBox->value() );
   return;
}

void Window::setFPS( float fps )
{
   char fps_str[ 10 ];
   sprintf( fps_str, "%3.2f FPS", fps );
   fpsCounterLabel->setText( fps_str );
   return;
}
