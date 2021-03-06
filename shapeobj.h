#include "commontypes.h"

class ShapeObj
{
 public:
   ShapeObj( ShapeType shape_type );

   struct Node * node;
   bool drag_state = false;
   bool setCenter( QPointF center );
   void setSize( QSizeF size );
   void setPen( QPen pen );
   void setBrush( QBrush brush );
   void setVelocity( QPointF vel );
   void applyForce(QPointF force);
   QPointF getCenter( );
   QSizeF getSize( );
   QPen getPen( );
   QBrush getBrush( );
   QPainterPath getPath( );
   QPointF getVelocity( );
   QPointF getNetForce();
   ShapeType getShapeType();
   void draw( QPainter& painter );
   bool intersects( ShapeObj& other );
   QRectF getAABB( );

 private:
   QRectF aabb = QRectF( );
   ShapeType shape_type;
   QPointF velocity = QPoint( 0.0, 0.0 );
   QPointF net_force = QPoint(0.0, 0.0);
   QPointF center = QPoint( 0.0, 0.0 );
   QSizeF size = QSize( 0.0, 0.0 );
   QPen pen = QPen( Qt::blue );
   QBrush brush = QBrush( );
   QPainterPath path;
   bool updatePath( );
   bool updateAABB( );
};
