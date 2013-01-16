#include "inputwidget.h"
#include "calthread.h"
#include <QtGui>

InputWidget::InputWidget(QWidget * parent) : QWidget(parent)
{
    //Initialzing layout manager.
    QVBoxLayout * layout = new QVBoxLayout;
    this->setLayout(layout);

    //Initializing Line Edits.
    leftEdit = new QLineEdit;
    rightEdit = new QLineEdit;
    upEdit = new QLineEdit;
    downEdit = new QLineEdit;
    densEdit = new QLineEdit;
    dxiEdit = new QLineEdit;
    detaEdit = new QLineEdit;
    dtEdit = new QLineEdit;
    reyEdit = new QLineEdit;
    totalCycleEdit = new QLineEdit;
    singleCycleEdit = new QLineEdit;
    spotCycleEdit = new QLineEdit;
    QLabel * leftLabel = new QLabel(tr("Left Boundary:"));
    QLabel * rightLabel = new QLabel(tr("Right Boundary:"));
    QLabel * upLabel = new QLabel(tr("Up Boundary:"));
    QLabel * downLabel = new QLabel(tr("Down Boundary:"));
    QLabel * densLabel = new QLabel(tr("Line Density:"));
    QLabel * dxiLabel = new QLabel(tr("Delta Xi:"));
    QLabel * detaLabel = new QLabel(tr("Delta Eta:"));
    QLabel * dtLabel = new QLabel(tr("Delta t:"));
    QLabel * reyLabel = new QLabel(tr("Reynolds Number:"));
    QLabel * totalCycleLabel = new QLabel(tr("Total Cycle Count:"));
    QLabel * singleCycleLabel = new QLabel(tr("Single Cycle Count:"));
    QLabel * spotCycleLabel = new QLabel(tr("Spot Stain Count"));
    leftEdit->setText("-50");
    rightEdit->setText("300");
    upEdit->setText("50");
    downEdit->setText("-50");
    densEdit->setText("0.2");
    dxiEdit->setText("0.2");
    detaEdit->setText("0.2");
    dtEdit->setText("0.2");
    reyEdit->setText("20");
    totalCycleEdit->setText("40");
    singleCycleEdit->setText("1");
    spotCycleEdit->setText("1000");
    layout->addWidget(leftLabel);
    layout->addWidget(leftEdit);
    layout->addWidget(rightLabel);
    layout->addWidget(rightEdit);
    layout->addWidget(upLabel);
    layout->addWidget(upEdit);
    layout->addWidget(downLabel);
    layout->addWidget(downEdit);
    layout->addWidget(densLabel);
    layout->addWidget(densEdit);
    layout->addWidget(dxiLabel);
    layout->addWidget(dxiEdit);
    layout->addWidget(detaLabel);
    layout->addWidget(detaEdit);
    layout->addWidget(dtLabel);
    layout->addWidget(dtEdit);
    layout->addWidget(reyLabel);
    layout->addWidget(reyEdit);
    layout->addWidget(totalCycleLabel);
    layout->addWidget(totalCycleEdit);
    layout->addWidget(singleCycleLabel);
    layout->addWidget(singleCycleEdit);
    layout->addWidget(spotCycleLabel);
    layout->addWidget(spotCycleEdit);

    //Adding a stretchable space to the  end of the layout
    layout->addStretch();
}

CalThread * InputWidget::constructThread(QObject * parent)
{
    bool ok;
    int l = leftEdit->text().toInt(&ok);

    if (!ok) {
        QMessageBox::warning(this, tr("Warning"), tr("Invalid value:"
                             "Left Boundary"));
        return NULL;
    }

    int r = rightEdit->text().toInt(&ok);

    if (!ok) {
        QMessageBox::warning(this, tr("Warning"), tr("Invalid value:"
                             "Right Boundary"));
        return NULL;
    }

    int u = upEdit->text().toInt(&ok);

    if (!ok) {
        QMessageBox::warning(this, tr("Warning"), tr("Invalid value:"
                             "Up Boundary"));
        return NULL;
    }

    int d = downEdit->text().toInt(&ok);

    if (!ok) {
        QMessageBox::warning(this, tr("Warning"), tr("Invalid value:"
                             "Down Boundary"));
        return NULL;
    }

    double dens = densEdit->text().toDouble(&ok);

    if (!ok || dens <= 0) {
        QMessageBox::warning(this, tr("Warning"), tr("Invalid value:"
                             "Density"));
        return NULL;
    }

    double dxi = dxiEdit->text().toDouble(&ok);

    if (!ok || dxi <= 0) {
        QMessageBox::warning(this, tr("Warning"), tr("Invalid value:"
                             "Delta Xi"));
        return NULL;
    }

    double deta = detaEdit->text().toDouble(&ok);

    if (!ok || deta <= 0) {
        QMessageBox::warning(this, tr("Warning"), tr("Invalid value:"
                             "Delta Eta"));
        return NULL;
    }

    double dt = dtEdit->text().toDouble(&ok);

    if (!ok || dt <= 0) {
        QMessageBox::warning(this, tr("Warning"), tr("Invalid value:"
                             "Delta t"));
        return NULL;
    }

    double rey = reyEdit->text().toDouble(&ok);

    if (!ok || rey <= 0) {
        QMessageBox::warning(this, tr("Warning"), tr("Invalid value:"
                             "Reynolds Number"));
        return NULL;
    }

    int total = totalCycleEdit->text().toInt(&ok);

    if (!ok || total <= 0) {
        QMessageBox::warning(this, tr("Warning"), tr("Invalid value:"
                             "Total Cycle Count"));
        return NULL;
    }

    int single = singleCycleEdit->text().toInt(&ok);

    if (!ok || single <= 0) {
        QMessageBox::warning(this, tr("Warning"), tr("Invalid value:"
                             "Single Cycle Count"));
        return NULL;
    }

    int spot = spotCycleEdit->text().toInt(&ok);

    if (!ok || spot <= 0) {
        QMessageBox::warning(this, tr("Warning"), tr("Invalid value:"
                             "Spot Stain Count"));
        return NULL;
    }

    return new CalThread(parent,
                         l,
                         r,
                         u,
                         d,
                         dens,
                         dxi,
                         deta,
                         dt,
                         rey,
                         total,
                         single,
                         spot);
}

InputWidget::~InputWidget()
{

}

void InputWidget::changeMode(int index)
{

}

