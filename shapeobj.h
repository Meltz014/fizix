#include "commontypes.h"
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QFlags>

class ShapeObj
{
 public:
   bool drag_state = false;
   ShapeObj( ShapeType shape_type );
   void setCenter( QPointF center );
   void setSize( QSizeF size );
   void setPen( QPen pen );
   void setBrush( QBrush brush );
   void setVelocity( QPointF vel );
   QPointF getCenter( );
   QSizeF getSize( );
   QPen getPen( );
   QBrush getBrush( );
   QPainterPath getPath( );
   QPointF getVelocity( );
   ShapeType getShapeType( );
   void draw( QPainter& painter );
   bool intersects( ShapeObj& other );


 private:
   ShapeType shape_type;
   QPointF velocity = QPoint( 0.0, 0.0 );
   QPointF center = QPoint( 0.0, 0.0 );
   QSizeF size = QSize( 0.0, 0.0 );
   QPen pen = QPen( Qt::blue );
   QBrush brush = QBrush( );
   QPainterPath path;
   void updatePath( );
};
