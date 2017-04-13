#include "commontypes.h"
#include "animator.h"

class RenderArea : public QWidget
{
   Q_OBJECT

 public:
   RenderArea( QWidget *parent = 0 );

   QSize minimumSizeHint( ) const override;
   QSize sizeHint( ) const override;
   void setNewShape( ShapeType new_shape );
   void removeShape( uint32 shape_idx );

 public slots:
   void addShape( ShapeObj new_shape );

 protected:
   void paintEvent( QPaintEvent *event ) override;
   void mousePressEvent( QMouseEvent *event ) override;
   void mouseMoveEvent( QMouseEvent *event ) override;
   void mouseReleaseEvent( QMouseEvent *event ) override;
   void timerEvent( QTimerEvent *event ) override;

 private:
   ShapeType new_shape_type = FCirc;
   ShapeObj *drag_shape;
   std::vector<ShapeObj> all_shapes;
   Animator animator;
   QPointF drag_offset = QPointF( 0.0, 0.0 );
   QTime drag_timer = QTime( );

   struct Node * aabb_tree_root;
   bool shapeOutOfBounds( uint32 shape_idx );
};



