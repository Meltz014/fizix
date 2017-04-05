#include <QDebug>
#include "renderarea.h"
#include "window.h"
#include <QtWidgets>

Window::Window( )
{
   qDebug( ) << "Hello";
   renderArea = new RenderArea;

   penWidthSpinBox = new QSpinBox;
   penWidthSpinBox->setRange(0, 20);
   penWidthSpinBox->setSpecialValueText(tr("0 (cosmetic pen)"));

   penWidthLabel = new QLabel(tr("Pen &Width:"));
   penWidthLabel->setBuddy(penWidthSpinBox);

   // connect the param controls with associated slots
   // signal on valueChanged() of spin box, call penChanged() of this window
   connect( penWidthSpinBox, SIGNAL( valueChanged( int ) ), this, SLOT( penChanged(  ) ) );

   // create layout
   QGridLayout *mainLayout = new QGridLayout;
   mainLayout->setColumnStretch( 0, 1 );
   mainLayout->addWidget( renderArea, 0, 0, 1, 4 );
   mainLayout->addWidget( penWidthLabel, 1, 0, Qt::AlignRight );
   mainLayout->addWidget( penWidthSpinBox, 1, 1 );
   setLayout( mainLayout );

   penChanged(  );
   setWindowTitle( tr( "Basic Drawing" ) );
}

void Window::penChanged(  )
{
   return;
}
