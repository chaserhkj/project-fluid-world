#include "controlwidget.h"
#include <QtGui>

ControlWidget::ControlWidget(QWidget * parent): QWidget(parent)
{
    //Initializing layout manager.
    QVBoxLayout * layout = new QVBoxLayout;
    this->setLayout(layout);
    
    //Initializing mode chooser.
    QComboBox * box = new QComboBox;
    QStringList list;
    list<<"1"<<"2";
    box->addItems(list);
    //Adding it to the layout.
    layout->addWidget(box);

    //Adding a stretchable space to the  end of the layout
    layout->addStretch();
}

ControlWidget::~ControlWidget()
{
    
}
