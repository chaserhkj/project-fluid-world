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

    //Initializing display factor selecter.
    QLabel * spinLabel = new QLabel(tr("Display Factor:"));
    QDoubleSpinBox * spinBox = new QDoubleSpinBox;
    spinBox->setMinimum(0.5);
    spinBox->setMaximum(99.9);
    spinBox->setSingleStep(0.5);
    spinBox->setValue(10.0);
    QHBoxLayout * spinLayout = new QHBoxLayout;
    spinLayout->addWidget(spinLabel);
    spinLayout->addWidget(spinBox);
    layout->addLayout(spinLayout);

    //Initialzing display control buttons.
    QLabel * displayLabel = new QLabel(tr("Display Control:"));
    QPushButton * previousBtn = new QPushButton(tr("&Previous Graph"));
    QPushButton * nextBtn = new QPushButton(tr("&Next Graph"));
    QHBoxLayout * btnLayout = new QHBoxLayout;
    btnLayout->addWidget(previousBtn);
    btnLayout->addWidget(nextBtn);
    layout->addWidget(displayLabel);
    layout->addLayout(btnLayout);
    //Adding a stretchable space to the  end of the layout
    layout->addStretch();

    //Initializing calculation control buttons.
    QLabel * calculationLabel = new QLabel(tr("Calculation Control:"));
    QPushButton * startBtn = new QPushButton(tr("&Start Calculation"));
    QPushButton * pauseBtn = new QPushButton(tr("&Pause Calculation"));
    pauseBtn -> setEnabled(false);
    QPushButton * stopBtn = new QPushButton(tr("S&top Calculation"));
    stopBtn -> setEnabled(false);
    //Adding them to the layout.
    layout->addWidget(calculationLabel);
    layout->addWidget(startBtn);
    layout->addWidget(pauseBtn);
    layout->addWidget(stopBtn);
    
    
    //Connecting signals for event handling.
    QObject::connect(modeChooser,
                     SIGNAL(currentIndexChanged(int)),
                     this,
                     SIGNAL(modeChose(int)));
    QObject::connect(spinBox,
                     SIGNAL(valueChanged(double)),
                     this,
                     SIGNAL(factorChanged(double)));
    QObject::connect(previousBtn,
                     SIGNAL(clicked()),
                     this,
                     SIGNAL(previousGraphClicked()));
    QObject::connect(nextBtn,
                     SIGNAL(clicked()),
                     this,
                     SIGNAL(nextGraphClicked()));
    QObject::connect(startBtn, SIGNAL(clicked()),
                     this, SIGNAL(startClicked()));
    QObject::connect(pauseBtn, SIGNAL(clicked()),
                     this, SIGNAL(pauseClicked()));
    QObject::connect(stopBtn, SIGNAL(clicked()),
                     this, SIGNAL(stopClicked()));
    
    
}

ControlWidget::~ControlWidget()
{

}
