#include "commontypes.h"
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QFlags>

class ShapeObj
{
 public:
   ShapeObj( ShapeType shape_type );
   void setCenter( QPoint center );
   void setSize( QSize size );
   void setPen( QPen pen );
   void setBrush( QBrush brush );
   QPoint getCenter( );
   QSize getSize( );
   QPen getPen( );
   QBrush getBrush( );
   QPainterPath getPath( );

 private:
   ShapeType shape_type;
   QPoint center = QPoint( 0,0 );
   QSize size = QSize( 0, 0 );
   QPen pen = QPen( Qt::blue );
   QBrush brush = QBrush( );
   QPainterPath path;
   void updatePath( );
};
