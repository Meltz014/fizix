#include <QWidget>

class QCheckBox;
class QComboBox;
class QLabel;
class QSpinBox;

class RenderArea;

class Window : public QWidget
{
  Q_OBJECT

 public:
   Window( );
   void setFPS( float fps );

 private slots:
   void penChanged(  );

 private:
   RenderArea *renderArea;
   QLabel *newShapeLabel;
   QSpinBox *newShapeSpinBox;
   QLabel *fpsCounterLabel;
};
