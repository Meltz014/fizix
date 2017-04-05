#include "renderarea.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QFlags>

RenderArea::RenderArea( QWidget * parent )
   : QWidget( parent )
{
   drag_shape = NULL;
   setBackgroundRole( QPalette::Base );
   setAutoFillBackground( true );
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize RenderArea::sizeHint() const
{
    return QSize(400, 200);
}

void RenderArea::addShape( ShapeObj new_shape )
{
   this->all_shapes.push_back( new_shape );
   update( );
}

void RenderArea::paintEvent( QPaintEvent * /* event */ )
{
   QPainter painter( this );

   for ( int shape_idx = 0; shape_idx < ( int )all_shapes.size( ); shape_idx++ )
   {
      // draw the shape
      painter.save( );
      painter.setPen( all_shapes[ shape_idx ].getPen( ) );
      painter.setBrush( all_shapes[ shape_idx ].getBrush( ) );
      painter.setRenderHint( QPainter::Antialiasing, true );
      painter.drawPath( all_shapes[ shape_idx ].getPath( ) );

      // fill any intersections
      for ( int other_idx = 0; other_idx < ( int )all_shapes.size( ); other_idx++ )
      {
         if ( other_idx == shape_idx )
         {
            continue;
         }
         if ( all_shapes[ shape_idx ].getPath( ).intersects( all_shapes[ other_idx ].getPath( ) ) )
         {
            QPainterPath intersection = all_shapes[ shape_idx ].getPath( ) & all_shapes[ other_idx ].getPath( );
            painter.fillPath( intersection, Qt::blue );
         }
      }
      painter.restore( );
   }

   // Draw border around render area
   painter.setRenderHint(QPainter::Antialiasing, false);
   painter.setPen(palette().dark().color());
   painter.setBrush(Qt::NoBrush);
   painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}

void RenderArea::mousePressEvent( QMouseEvent *event )
{
   Qt::MouseButton button = event->button(  );
   int x_coord = event->x( );
   int y_coord = event->y( );

   qDebug( ) << "mousePressEvent" << x_coord << "y:" << y_coord;

   if ( button & Qt::LeftButton )
   {
      qDebug( ) << "left";
      for ( int i = 0; i < ( int )all_shapes.size( ); i++ )
      {
         if ( all_shapes[ i ].getPath( ).contains( event->pos( ) ) )
         {
            drag_shape = &all_shapes[ i ];
            qDebug( ) << "Ofsset" << drag_shape->getCenter( ) - event->pos( );
            drag_offset = drag_shape->getCenter( ) - event->pos( );
            break;
         }
         else
         {
            drag_shape = NULL;
         }
      }
   }
   else if ( button & Qt::RightButton )
   {
      ShapeObj new_shape = ShapeObj( FCirc );
      qDebug( ) << "right";
      new_shape.setPen( QPen( QPen( Qt::blue, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) ) );
      new_shape.setBrush( QBrush( ) );
      new_shape.setCenter( QPoint( x_coord, y_coord ) );
      new_shape.setSize( QSize( 80, 80 ) );
      addShape( new_shape );
      drag_offset = QPoint( 0, 0 );
      drag_shape = NULL;
      update( );
   }
}

void RenderArea::mouseMoveEvent( QMouseEvent *event )
{
   if ( drag_shape )
   {
      drag_shape->setCenter( event->pos( ) + drag_offset );
      update( );
   }
}
