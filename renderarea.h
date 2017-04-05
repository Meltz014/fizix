#include "shapeobj.h"
#include <QPixmap>
#include <QWidget>
#include <vector>

class RenderArea : public QWidget
{
   Q_OBJECT

 public:
   RenderArea( QWidget *parent = 0 );

   QSize minimumSizeHint( ) const override;
   QSize sizeHint( ) const override;

 public slots:
   void addShape( ShapeObj new_shape );

 protected:
   void paintEvent( QPaintEvent *event ) override;
   void mousePressEvent( QMouseEvent *event ) override;
   void mouseMoveEvent( QMouseEvent *event ) override;

 private:
   ShapeObj *drag_shape;
   std::vector<ShapeObj> all_shapes;
   QPoint drag_offset = QPoint( 0, 0 );
};



