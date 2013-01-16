#include "inputwidget.h"
#include <QtGui>

InputWidget::InputWidget(QWidget * parent) : QWidget(parent)
{
    //Initialzing layout manager.
    QVBoxLayout * layout = new QVBoxLayout;
    this->setLayout(layout);

    //Initialzing button.
    QPushButton * startBtn = new QPushButton(tr("&Start Calculate"));
    //Adding them to the layout.
    layout->addWidget(startBtn);
    //Connecting signals for buttons.
    QObject::connect(startBtn, SIGNAL(clicked()),
                     this, SIGNAL(startClicked()));

    //Adding a stretchable space to the  end of the layout
    layout->addStretch();
}

InputWidget::~InputWidget()
{

}

void InputWidget::changeMode(int index)
{

}

