#include "inputwidget.h"
#include <QtGui>

InputWidget::InputWidget(QWidget * parent): QWidget(parent)
{
    //Initialzing layout manager.
    QVBoxLayout * layout = new QVBoxLayout;
    this->setLayout(layout);

    //Initialzing value inputer.
    QLineEdit  * valueEdit=new QLineEdit;
    //Adding it to the layout.
    layout->addWidget(valueEdit);

    //Adding a stretchable space to the  end of the layout
    layout->addStretch();
}

InputWidget::~InputWidget()
{
    
}
