#include "renderarea.h"
#include "window.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QFlags>
#include <stdlib.h>
#include <time.h>

RenderArea::RenderArea( QWidget * parent )
   : QWidget( parent ), animator( &all_shapes )
{
   drag_shape = NULL;
   setBackgroundRole( QPalette::Base );
   setAutoFillBackground( true );

   srand( time( NULL ) );
   drag_timer.start( );
   // start timer
   startTimer( 5 );
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

void RenderArea::timerEvent( QTimerEvent * /* event */ )
{
   this->animator.animateNextFrame( rect( ) );
   update( );
   return;
}

void RenderArea::setNewShape( ShapeType new_shape )
{
   this->new_shape_type = new_shape;
}

void RenderArea::paintEvent( QPaintEvent * /* event */ )
{
   QPainter painter( this );

   // remove any shapes that are outside of the bounds
   for ( int shape_idx = 0; shape_idx < ( int )all_shapes.size( ); shape_idx++ )
   {
      if ( !all_shapes[ shape_idx ].drag_state ) // don't check shapes being dragged
      {
         // check intersection of canvas bounds and shape bounding rect.
         // if no intersection, shape is out of canvas and should be removed
         if ( !QRectF( rect( ) ).intersects( all_shapes[ shape_idx ].getPath( ).boundingRect( ) ) )
         {
            qDebug( ) << "remove" << shape_idx;
            std::swap( all_shapes[ shape_idx ], all_shapes.back( ) );
            all_shapes.pop_back( );
            shape_idx--;
         }
      }
   }

   for ( int shape_idx = 0; shape_idx < ( int )all_shapes.size( ); shape_idx++ )
   {
      painter.save( );
      all_shapes[ shape_idx ].draw( painter );

      // fill any intersections
      for ( int other_idx = 0; other_idx < ( int )all_shapes.size( ); other_idx++ )
      {
         if ( other_idx == shape_idx )
         {
            continue;
         }
         if ( all_shapes[ shape_idx ].intersects( all_shapes[ other_idx ] ) )
         {
            QPainterPath intersection = all_shapes[ shape_idx ].getPath( ) & all_shapes[ other_idx ].getPath( );
            painter.fillPath( intersection, Qt::blue );

            painter.save( );
            painter.setPen( QPen( Qt::cyan, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
            painter.drawEllipse( intersection.pointAtPercent( 0.0 ), 5, 5 );
            painter.setPen( QPen( Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
            painter.drawEllipse( intersection.pointAtPercent( 0.25 ), 5, 5 );
            painter.setPen( QPen( Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
            painter.drawEllipse( intersection.pointAtPercent( 0.50 ), 5, 5 );
            painter.setPen( QPen( Qt::green, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
            painter.drawEllipse( intersection.pointAtPercent( 0.75 ), 5, 5 );

            painter.setPen( QPen( Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
            painter.drawLine( intersection.pointAtPercent( 0.0 ), intersection.pointAtPercent( 0.50 ) );
            painter.drawLine( intersection.pointAtPercent( 0.25 ), intersection.pointAtPercent( 0.75 ) );
            painter.restore( );
         }
      }
      painter.restore( );
   }

   // Draw border around render area
   painter.setRenderHint(QPainter::Antialiasing, false);
   painter.setPen(palette().dark().color());
   painter.setBrush(Qt::NoBrush);
   painter.drawRect(QRect(0, 0, width() - 1, height() - 1));

   // update FPS label
   ( ( Window * ) parentWidget( ) )->setFPS( animator.getFPS( ) );
}

void RenderArea::mousePressEvent( QMouseEvent *event )
{
   Qt::MouseButton button = event->button(  );
   int x_coord = event->x( );
   int y_coord = event->y( );
   float randX = 0.0F;
   float randY = 0.0F;

   qDebug( ) << "FPS" << animator.getFPS( );
   if ( button & Qt::LeftButton )
   {
      for ( int i = 0; i < ( int )all_shapes.size( ); i++ )
      {
         if ( all_shapes[ i ].getPath( ).contains( event->pos( ) ) )
         {
            // restart drag timer
            drag_timer.restart( );
            drag_shape = &all_shapes[ i ];
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
      ShapeObj new_shape = ShapeObj( this->new_shape_type );
      new_shape.setPen( QPen( Qt::blue, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
      new_shape.setBrush( QBrush( ) );
      new_shape.setCenter( QPoint( x_coord, y_coord ) );
      new_shape.setSize( QSize( 80, 80 ) );
      randX = ( float ) ( rand( ) % 20 ) - 10;
      randY = ( float ) ( rand( ) % 20 ) - 10;
      new_shape.setVelocity( QPointF( randX, randY ) );
      addShape( new_shape );
      drag_offset = QPoint( 0, 0 );
      drag_shape = NULL;
      update( );
   }
}

void RenderArea::mouseMoveEvent( QMouseEvent *event )
{
   QPointF new_pos;
   QPointF new_vel;
   if ( drag_shape )
   {
      drag_shape->drag_state = true;
      // calculate new position
      new_pos = event->pos( ) + drag_offset;
      // calculate velocity
      // v = d/t
      new_vel = ( new_pos - drag_shape->getCenter( ) ) / ( drag_timer.elapsed( ) / 1000.0F );

      // set properties
      drag_shape->setCenter( new_pos );
      drag_shape->setVelocity( new_vel );

      // restart drag timer
      drag_timer.restart( );
      update( );
   }
}

void RenderArea::mouseReleaseEvent( QMouseEvent *event )
{
   if ( drag_shape )
   {
      drag_shape->drag_state = false;
      drag_shape = NULL;
   }
}
