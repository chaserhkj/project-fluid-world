#include "displaywidget.h"
#include <QtOpenGL>
#include "projectmainwindow.h"
#include "calthread.h"
#include <interface.h>

DisplayWidget::DisplayWidget(QWidget * parent): QGLWidget(parent)
{
}

DisplayWidget::~DisplayWidget()
{
}

void DisplayWidget::updateGraph()
{
    ProjectMainWindow * win = qobject_cast<ProjectMainWindow *>(this->parent());
    Project * pro = win->getThread()->getProject();
    makeCurrent();
    glClear(GL_COLOR_BUFFER_BIT);
    DataVariant * data = pro->getData(Project::NumberType);
    int n = data->getNumber();
    delete data;
    double f = 1.0/n;
    int i;
    for (i = 0; i < n; ++i)
    {
        glColor3f(1-f*i,0.0,f*i);
        data = pro->getData(Project::SpotType, i);
        glBegin(GL_LINES);
        glVertex3f(data->getX(),data->getY(),0.0);
        while (data->next()){
            glVertex3f(data->getX(),data->getY(),0.0);            
            glEnd();
            glBegin(GL_LINES);
            glVertex3f(data->getX(),data->getY(),0.0);
        }
        glEnd();
        glFlush();
    }
}

void DisplayWidget::initializeGL()
{
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glScalef(2.0/25,1.0/5,1.0);
    glTranslatef(-7.5,0.0,0.0);
}

void DisplayWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
}

void DisplayWidget::resizeGL(int w, int h)
{
    
}
