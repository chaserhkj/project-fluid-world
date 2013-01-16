#include "controlwidget.h"
#include <QtGui>

ControlWidget::ControlWidget(QWidget * parent) : QWidget(parent)
{
    //Initializing layout manager.
    QVBoxLayout * layout = new QVBoxLayout;
    this->setLayout(layout);

    //Initializing mode chooser.
    QLabel * modeLabel = new QLabel(tr("Please choose mode:"));
    QComboBox * modeChooser = new QComboBox;
    QStringList list;
    int i;
    list << "cylinder";
    modeChooser->addItems(list);
    //Adding them to the layout.
    layout->addWidget(modeLabel);
    layout->addWidget(modeChooser);

    //Adding a stretchable space to the  end of the layout
    layout->addStretch();

    //Connecting signals for event handling.
    QObject::connect(modeChooser,
                     SIGNAL(currentIndexChanged(int)),
                     this,
                     SIGNAL(modeChose(int)));

}

ControlWidget::~ControlWidget()
{

}
